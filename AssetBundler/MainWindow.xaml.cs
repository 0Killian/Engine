using AssetBundler.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace AssetBundler
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private ObservableCollection<AssetBundleContent> _assetBundles = new ObservableCollection<AssetBundleContent>();
        public ObservableCollection<AssetBundleContent> AssetBundles
        {
            get => _assetBundles;
            set
            {
                _assetBundles = value;
                OnPropertyChanged();
            }
        }

        private List<int> _selectedContents = new List<int>();

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        private void OpenAssetBundle_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new Microsoft.Win32.OpenFileDialog();
            dialog.DefaultExt = ".bundle";
            dialog.Filter = "Asset Bundle (.bundle)|*.bundle";

            if (dialog.ShowDialog() == true)
            {
                AssetBundles = Models.AssetBundleContent.OpenFromFile(dialog.FileName);
            }
        }

        private void SaveAssetBundle_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new Microsoft.Win32.SaveFileDialog();
            dialog.DefaultExt = ".bundle";
            dialog.Filter = "Asset Bundle (.bundle)|*.bundle";

            if (dialog.ShowDialog() == true)
            {
                Models.AssetBundleContent.SaveToFile(dialog.FileName, AssetBundles);
            }
        }

        private void AssetBundleContent_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            foreach (AssetBundleContent content in e.AddedItems)
            {
                _selectedContents.Add(AssetBundles.IndexOf(content));
            }

            foreach (AssetBundleContent content in e.RemovedItems)
            {
                _selectedContents.Remove(AssetBundles.IndexOf(content));
            }
        }

        private void RemoveContent_Click(object sender, RoutedEventArgs e)
        {
            foreach (int index in _selectedContents.OrderByDescending(x => x))
            {
                AssetBundles.RemoveAt(index);
            }

            _selectedContents.Clear();
        }

        private void AddContent_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new Microsoft.Win32.OpenFileDialog();
            dialog.Filter = "Pipeline (.hlsl)|*.hlsl";

            if (dialog.ShowDialog() == true)
            {
                AssetBundles.Add(Models.AssetBundleContent.LoadAsset(dialog.FileName));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public void OnPropertyChanged([CallerMemberName] string propertyName = null)
            => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}
