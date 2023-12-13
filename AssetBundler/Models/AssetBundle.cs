using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;

namespace AssetBundler.Models
{
    public enum AssetType
    {
        Pipeline = 0
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    [Serializable]
    public struct AssetBundleHeader
    {
        public byte Version;
        public UInt32 AssetCount;
        public UInt32 StringTableOffset;
        public UInt32 StringTableSize;
        public UInt32 PreviousBundleString;
        public UInt32 NextBundleString;
        public UInt32 AssetTableOffset;
        public UInt32 AssetTableSize;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    [Serializable]
    public struct AssetHeader
    {
        public UInt32 NameString;
        public UInt32 Size;
        public UInt32 Type;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    [Serializable]
    public struct PipelineHeader
    {
        public UInt32 VertexDebugSize;
        public UInt32 VertexReleaseSize;
        public UInt32 PixelDebugSize;
        public UInt32 PixelReleaseSize;
    }

    public class AssetBundleContent : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged([CallerMemberName] string propertyName = null)
            => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

        private string _name;
        private int _size;
        private AssetType _assetType;
        private byte[] _data;

        public string Name
        {
            get => _name;
            set
            {
                _name = value;
                OnPropertyChanged();
            }
        }

        public int Size
        {
            get => _size;
            set
            {
                _size = value;
                OnPropertyChanged();
            }
        }

        public AssetType Type
        {
            get => _assetType;
            set
            {
                _assetType = value;
                OnPropertyChanged();
            }
        }

        public AssetBundleContent(string name, int size, AssetType type, byte[] data)
        {
            Name = name;
            Size = size;
            Type = type;
            _data = data;
        }

        public static ObservableCollection<AssetBundleContent> OpenFromFile(string path)
        {
            var contents = new ObservableCollection<AssetBundleContent>();

            // Open file
            using(FileStream fs = File.OpenRead(path))
            {
                // Read header
                AssetBundleHeader header = new AssetBundleHeader();
                IntPtr intPtr = IntPtr.Zero;
                byte[] bytes = new byte[Marshal.SizeOf(header)];
                fs.Read(bytes, 0, Marshal.SizeOf(header));

                try
                {
                    intPtr = Marshal.AllocHGlobal(Marshal.SizeOf(header));
                    Marshal.Copy(bytes, 0, intPtr, Marshal.SizeOf(header));
                    header = (AssetBundleHeader)Marshal.PtrToStructure(intPtr, typeof(AssetBundleHeader));
                }
                finally
                {
                    Marshal.FreeHGlobal(intPtr);
                }

                if(header.Version != 1)
                {
                    throw new Exception("Unsupported asset bundle version");
                }

                // Read string table
                fs.Seek(header.StringTableOffset, SeekOrigin.Begin);
                List<string> stringTable = new List<string>();
                bytes = new byte[header.StringTableSize];
                fs.Read(bytes, 0, (int)header.StringTableSize);
                string currentString = "";

                for (int i = 0; i < header.StringTableSize; i++)
                {
                    if (bytes[i] == 0)
                    {
                        stringTable.Add(currentString);
                        currentString = "";
                    }
                    else
                    {
                        currentString += (char)bytes[i];
                    }
                }

                if(header.PreviousBundleString != 0)
                {
                    contents.Concat(OpenFromFile(Directory.GetParent(path).ToString() + stringTable[(int)header.PreviousBundleString]));
                }

                // Read asset table
                fs.Seek(header.AssetTableOffset, SeekOrigin.Begin);
                for (int i = 0; i < header.AssetCount; i++)
                {
                    AssetHeader assetHeader = new AssetHeader();
                    intPtr = IntPtr.Zero;

                    bytes = new byte[Marshal.SizeOf(assetHeader)];
                    fs.Read(bytes, 0, Marshal.SizeOf(assetHeader));

                    try
                    {
                        intPtr = Marshal.AllocHGlobal(Marshal.SizeOf(assetHeader));
                        Marshal.Copy(bytes, 0, intPtr, Marshal.SizeOf(assetHeader));
                        assetHeader = (AssetHeader)Marshal.PtrToStructure(intPtr, typeof(AssetHeader));
                    }
                    finally
                    {
                        Marshal.FreeHGlobal(intPtr);
                    }

                    byte[] data = new byte[assetHeader.Size];
                    fs.Read(data, 0, (int)assetHeader.Size);

                    contents.Add(new AssetBundleContent(stringTable[(int)assetHeader.NameString], (int)assetHeader.Size, (AssetType)assetHeader.Type, data));
                }

                if(header.NextBundleString != 0)
                {
                    contents.Concat(OpenFromFile(Directory.GetParent(path).ToString() + stringTable[(int)header.NextBundleString]));
                }
            }

            return contents;
        }

        public static void SaveToFile(string path, ObservableCollection<AssetBundleContent> contents)
        {
            // Open file
            using(FileStream fs = File.OpenWrite(path))
            {
                // Write header
                AssetBundleHeader header = new AssetBundleHeader();
                header.Version = 1;
                header.AssetCount = (UInt32)contents.Count;
                header.StringTableOffset = (UInt32)Marshal.SizeOf(header);
                header.StringTableSize = 0;
                header.PreviousBundleString = 0;
                header.NextBundleString = 0;
                header.AssetTableOffset = 0;
                header.AssetTableSize = 0;
                
                // Write string table
                List<string> stringTable = new List<string>();
                foreach (AssetBundleContent content in contents)
                {
                    stringTable.Add(content.Name);
                }

                List<byte> result = new List<byte>();
                foreach (string str in stringTable)
                {
                    byte[] bytes = new byte[str.Length + 1];
                    for (int i = 0; i < str.Length; i++)
                    {
                        bytes[i] = (byte)str[i];
                    }
                    bytes[str.Length] = 0;
                    result.AddRange(bytes);
                }

                header.StringTableSize = (UInt32)result.Count;
                header.AssetTableOffset = header.StringTableOffset + header.StringTableSize;

                IntPtr intPtr = IntPtr.Zero;

                // Write asset table
                foreach (AssetBundleContent content in contents)
                {
                    AssetHeader assetHeader = new AssetHeader();
                    assetHeader.NameString = (UInt32)stringTable.IndexOf(content.Name);
                    assetHeader.Size = (UInt32)content.Size;
                    assetHeader.Type = (UInt32)content.Type;

                    intPtr = IntPtr.Zero;
                    try
                    {
                        intPtr = Marshal.AllocHGlobal(Marshal.SizeOf(assetHeader));
                        Marshal.StructureToPtr(assetHeader, intPtr, false);
                        byte[] bytes = new byte[Marshal.SizeOf(assetHeader)];
                        Marshal.Copy(intPtr, bytes, 0, Marshal.SizeOf(assetHeader));

                        result.AddRange(bytes);
                    }
                    finally
                    {
                        Marshal.FreeHGlobal(intPtr);
                    }

                    result.AddRange(content._data);
                }

                header.AssetTableSize = (UInt32)result.Count - header.StringTableSize;

                intPtr = IntPtr.Zero;

                try
                {
                    intPtr = Marshal.AllocHGlobal(Marshal.SizeOf(header));
                    Marshal.StructureToPtr(header, intPtr, false);
                    byte[] bytes = new byte[Marshal.SizeOf(header)];
                    Marshal.Copy(intPtr, bytes, 0, Marshal.SizeOf(header));

                    fs.Write(bytes, 0, Marshal.SizeOf(header));
                }
                finally
                {
                    Marshal.FreeHGlobal(intPtr);
                }
                
                fs.Write(result.ToArray(), 0, result.Count);
            }
        }

        public static AssetBundleContent LoadAsset(string path)
        {
            switch(Path.GetExtension(path))
            {
            case ".hlsl":
                return LoadPipeline(path);
            default:
                throw new Exception("Unsupported asset type");
            }
        }

        private static AssetBundleContent LoadPipeline(string path)
        {
            // Compile shader
            string source = File.ReadAllText(path);
            byte[] vertexDebug = SharpDX.D3DCompiler.ShaderBytecode.Compile(
                source,
                "VSMain",
                "vs_5_0",
                SharpDX.D3DCompiler.ShaderFlags.Debug | SharpDX.D3DCompiler.ShaderFlags.SkipOptimization | SharpDX.D3DCompiler.ShaderFlags.EnableStrictness,
                SharpDX.D3DCompiler.EffectFlags.None,
                "VertexShader");

            byte[] vertexRelease = SharpDX.D3DCompiler.ShaderBytecode.Compile(
                source,
                "VSMain",
                "vs_5_0",
                SharpDX.D3DCompiler.ShaderFlags.EnableStrictness,
                SharpDX.D3DCompiler.EffectFlags.None,
                "VertexShader");

            byte[] pixelDebug = SharpDX.D3DCompiler.ShaderBytecode.Compile(
                source,
                "PSMain",
                "ps_5_0",
                SharpDX.D3DCompiler.ShaderFlags.Debug | SharpDX.D3DCompiler.ShaderFlags.SkipOptimization | SharpDX.D3DCompiler.ShaderFlags.EnableStrictness,
                SharpDX.D3DCompiler.EffectFlags.None,
                "PixelShader");

            byte[] pixelRelease = SharpDX.D3DCompiler.ShaderBytecode.Compile(
                source,
                "PSMain",
                "ps_5_0",
                SharpDX.D3DCompiler.ShaderFlags.EnableStrictness,
                SharpDX.D3DCompiler.EffectFlags.None,
                "PixelShader");

            byte[] data = new byte[Marshal.SizeOf(typeof(PipelineHeader)) + vertexDebug.Length + vertexRelease.Length + pixelDebug.Length + pixelRelease.Length];
            IntPtr intPtr = IntPtr.Zero;
            try
            {
                intPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(PipelineHeader)));
                Marshal.StructureToPtr(new PipelineHeader()
                {
                    VertexDebugSize = (UInt32)vertexDebug.Length,
                    VertexReleaseSize = (UInt32)vertexRelease.Length,
                    PixelDebugSize = (UInt32)pixelDebug.Length,
                    PixelReleaseSize = (UInt32)pixelRelease.Length
                }, intPtr, false);
                Marshal.Copy(intPtr, data, 0, Marshal.SizeOf(typeof(PipelineHeader)));
            }
            finally
            {
                Marshal.FreeHGlobal(intPtr);
            }

            vertexDebug.CopyTo(data, Marshal.SizeOf(typeof(PipelineHeader)));
            vertexRelease.CopyTo(data, Marshal.SizeOf(typeof(PipelineHeader)) + vertexDebug.Length);
            pixelDebug.CopyTo(data, Marshal.SizeOf(typeof(PipelineHeader)) + vertexDebug.Length + vertexRelease.Length);
            pixelRelease.CopyTo(data, Marshal.SizeOf(typeof(PipelineHeader)) + vertexDebug.Length + vertexRelease.Length + pixelDebug.Length);

            string name = Path.GetFileNameWithoutExtension(path);

            return new AssetBundleContent(name, data.Length, AssetType.Pipeline, data);
        }
    }
}