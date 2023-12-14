#pragma once

#include "Containers/String.h"
#include "Containers/HashMap.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"
#include "Math/Mat4.h"

#include <ranges>
#include <stdexcept>

namespace NGN
{
	class VertexStructureElement
	{
	public:
		enum Type
		{
			Float,
			Float2,
			Float3,
			Float4,
			Mat4Row,
			Mat4Column,
			Int,
			Int2,
			Int3,
			Int4,
			Bool
		};

		VertexStructureElement() = default;

		VertexStructureElement(Type type, size_t count, bool normalized = false)
			: m_Type(type), m_Count(count), m_Normalized(normalized)
		{
		}

		Type m_Type = Float;
		size_t m_Count = 1;
		bool m_Normalized = false;
		size_t m_Offset = 0;

		size_t GetSize() const
		{
			switch (m_Type)
			{
			case Float: return sizeof(float)  * 1 * m_Count;
			case Float2: return sizeof(float) * 2 * m_Count;
			case Float3: return sizeof(float) * 3 * m_Count;
			case Float4: return sizeof(float) * 4 * m_Count;
			case Mat4Row:
			case Mat4Column: return sizeof(float) * 16 * m_Count;
			case Int: return sizeof(int)  * 1 * m_Count;
			case Int2: return sizeof(int) * 2 * m_Count;
			case Int3: return sizeof(int) * 3 * m_Count;
			case Int4: return sizeof(int) * 4 * m_Count;
			case Bool: return sizeof(bool) * m_Count;
			}
			throw std::runtime_error("Unknown VertexStructureElement type");
		}

		bool operator==(const VertexStructureElement& other) const
		{
			return m_Type == other.m_Type && m_Count == other.m_Count && m_Normalized == other.m_Normalized;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout(HashMap<String, VertexStructureElement> elements)
			: m_Elements(elements)
		{
			size_t offset = 0;
			for (auto& element : std::ranges::views::values(m_Elements))
			{
				element.m_Offset = offset;
				offset += element.GetSize();
			}
			m_Stride = offset;
		}

		const HashMap<String, VertexStructureElement>& GetStructure() const { return m_Elements; }
		size_t GetStride() const { return m_Stride; }


	protected:
		HashMap<String, VertexStructureElement> m_Elements;
		size_t m_Stride;
	};

	class VertexBuffer : public BufferLayout
	{
	public:
		VertexBuffer(HashMap<String, VertexStructureElement> elements)
			: BufferLayout(elements)
		{}

		size_t AddVertex(size_t count = 1)
		{
			if (count == 0)
				return 0;

			size_t index = m_Data.Size() / m_Stride;
			m_Data.Resize(m_Data.Size() + m_Stride * count);
			m_VertexCount += count;
			return index;
		}

		void SetElement(size_t vertexIndex, const String& elementName, float value, size_t elementIndex = 0)
		{
			if (vertexIndex >= m_VertexCount)
				throw std::runtime_error("Invalid vertex index");

			if(!m_Elements.ContainsKey(elementName))
				throw std::runtime_error("Invalid element name");

			auto& element = m_Elements[elementName];
			if (element.m_Type != VertexStructureElement::Float)
				throw std::runtime_error("Invalid element type");

			if (elementIndex >= element.m_Count)
				throw std::runtime_error("Invalid element index");

			size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(float);
			*(float*)&m_Data[offset] = value;
		}

		// TODO: Vec2<float>
		// void SetElement(size_t vertexIndex, const String& elementName, Vec2<float> value, size_t elementIndex = 0)
		// {
		//     if (vertexIndex >= m_VertexCount)
		//         throw std::runtime_error("Invalid vertex index");
		//
		//     if(!m_Elements.ContainsKey(elementName))
		//         throw std::runtime_error("Invalid element name");
		//
		//     auto& element = m_Elements[elementName];
		//     if (element.m_Type != VertexStructureElement::Float2)
		//         throw std::runtime_error("Invalid element type");
		//
		//     if (elementIndex >= element.m_Count)
		//         throw std::runtime_error("Invalid element index");
		//
		//     size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(float) * 2;
		//     *(float*)&m_Data[offset + sizeof(float) * 0] = value.x;
		//     *(float*)&m_Data[offset + sizeof(float) * 1] = value.y;
		// }

		void SetElement(size_t vertexIndex, const String& elementName, Math::Vec3<float> value, size_t elementIndex = 0)
		{
			if (vertexIndex >= m_VertexCount)
				throw std::runtime_error("Invalid vertex index");

			if(!m_Elements.ContainsKey(elementName))
		       throw std::runtime_error("Invalid element name");
		
		    auto& element = m_Elements[elementName];
		    if (element.m_Type != VertexStructureElement::Float3)
		        throw std::runtime_error("Invalid element type");
		
			if (elementIndex >= element.m_Count)
		        throw std::runtime_error("Invalid element index");
		
		    size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(float) * 3;
		    *(float*)&m_Data[offset + sizeof(float) * 0] = value.x;
		    *(float*)&m_Data[offset + sizeof(float) * 1] = value.y;
		    *(float*)&m_Data[offset + sizeof(float) * 2] = value.z;
		}

		void SetElement(size_t vertexIndex, const String& elementName, Math::Vec4<float> value, size_t elementIndex = 0)
		{
			if (vertexIndex >= m_VertexCount)
				throw std::runtime_error("Invalid vertex index");

		    if(!m_Elements.ContainsKey(elementName))
		        throw std::runtime_error("Invalid element name");
		
		    auto& element = m_Elements[elementName];
		    if (element.m_Type != VertexStructureElement::Float4)
		        throw std::runtime_error("Invalid element type");
		
		    if (elementIndex >= element.m_Count)
		        throw std::runtime_error("Invalid element index");
		
		    size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(float) * 4;
		    *(float*)&m_Data[offset + sizeof(float) * 0] = value.x;
		    *(float*)&m_Data[offset + sizeof(float) * 1] = value.y;
		    *(float*)&m_Data[offset + sizeof(float) * 2] = value.z;
		    *(float*)&m_Data[offset + sizeof(float) * 3] = value.w;
		}

		void SetElement(size_t vertexIndex, const String& elementName, int value, size_t elementIndex = 0)
		{
			if (vertexIndex >= m_VertexCount)
				throw std::runtime_error("Invalid vertex index");

			if(!m_Elements.ContainsKey(elementName))
				throw std::runtime_error("Invalid element name");

			auto& element = m_Elements[elementName];
			if (element.m_Type != VertexStructureElement::Int)
				throw std::runtime_error("Invalid element type");

			if (elementIndex >= element.m_Count)
				throw std::runtime_error("Invalid element index");

			size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(int);
			*(int*)&m_Data[offset] = value;
		}

		// TODO: Vec2<int>
		// void SetElement(size_t vertexIndex, String& elementName, Vec2<int> value, size_t elementIndex = 0)
		// {
		//     if (vertexIndex >= m_VertexCount)
		//         throw std::runtime_error("Invalid vertex index");
		//
		//     if(!m_Elements.ContainsKey(elementName))
		//         throw std::runtime_error("Invalid element name");
		//
		//     auto& element = m_Elements[elementName];
		//     if (element.m_Type != VertexStructureElement::Int2)
		//         throw std::runtime_error("Invalid element type");
		//
		//     if (elementIndex >= element.m_Count)
		//         throw std::runtime_error("Invalid element index");
		//
		//     size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(int) * 2;
		//     *(int*)&m_Data[offset + sizeof(int) * 0] = value.x;
		//     *(int*)&m_Data[offset + sizeof(int) * 1] = value.y;
		// }

		void SetElement(size_t vertexIndex, const String& elementName, Math::Vec3<int> value, size_t elementIndex = 0)
		{
			if (vertexIndex >= m_VertexCount)
				throw std::runtime_error("Invalid vertex index");

		    if(!m_Elements.ContainsKey(elementName))
		        throw std::runtime_error("Invalid element name");
		
		    auto& element = m_Elements[elementName];
		    if (element.m_Type != VertexStructureElement::Int3)
		         throw std::runtime_error("Invalid element type");
		
		    if (elementIndex >= element.m_Count)
		        throw std::runtime_error("Invalid element index");
		
		    size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(int) * 3;
		    *(int*)&m_Data[offset + sizeof(int) * 0] = value.x;
		    *(int*)&m_Data[offset + sizeof(int) * 1] = value.y;
		    *(int*)&m_Data[offset + sizeof(int) * 2] = value.z;
		}

		void SetElement(size_t vertexIndex, const String& elementName, Math::Vec4<int> value, size_t elementIndex = 0)
		{
			if (vertexIndex >= m_VertexCount)
				throw std::runtime_error("Invalid vertex index");

		    if(!m_Elements.ContainsKey(elementName))
		        throw std::runtime_error("Invalid element name");
		
		    auto& element = m_Elements[elementName];
		    if (element.m_Type != VertexStructureElement::Int4)
		        throw std::runtime_error("Invalid element type");
		
		    if (elementIndex >= element.m_Count)
		        throw std::runtime_error("Invalid element index");
		
		    size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(int) * 4;
		    *(int*)&m_Data[offset + sizeof(int) * 0] = value.x;
		    *(int*)&m_Data[offset + sizeof(int) * 1] = value.y;
		    *(int*)&m_Data[offset + sizeof(int) * 2] = value.z;
		    *(int*)&m_Data[offset + sizeof(int) * 3] = value.w;
		}

		void SetElement(size_t vertexIndex, const String& elementName, bool value, size_t elementIndex = 0)
		{
			if (vertexIndex >= m_VertexCount)
				throw std::runtime_error("Invalid vertex index");

			if(!m_Elements.ContainsKey(elementName))
				throw std::runtime_error("Invalid element name");

			auto& element = m_Elements[elementName];
			if (element.m_Type != VertexStructureElement::Bool)
				throw std::runtime_error("Invalid element type");

			if (elementIndex >= element.m_Count)
				throw std::runtime_error("Invalid element index");

			size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(bool);
			*(bool*)&m_Data[offset] = value;
		}

		void SetElement(size_t vertexIndex, const String& elementName, Math::Mat4<float, Math::ROW_MAJOR> value, size_t elementIndex = 0)
		{
			if (vertexIndex >= m_VertexCount)
				throw std::runtime_error("Invalid vertex index");

		    if(!m_Elements.ContainsKey(elementName))
		        throw std::runtime_error("Invalid element name");
		
		    auto& element = m_Elements[elementName];
		    if (element.m_Type != VertexStructureElement::Mat4Row)
		        throw std::runtime_error("Invalid element type");
		
		    if (elementIndex >= element.m_Count)
		        throw std::runtime_error("Invalid element index");
		
			size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(float) * 16;
			memcpy(&m_Data[offset], value.GetData(), sizeof(float) * 16);
		}

		void SetElement(size_t vertexIndex, const String& elementName, Math::Mat4<float, Math::COLUMN_MAJOR> value, size_t elementIndex = 0)
		{
			if (vertexIndex >= m_VertexCount)
				throw std::runtime_error("Invalid vertex index");

			if (!m_Elements.ContainsKey(elementName))
				throw std::runtime_error("Invalid element name");

			auto& element = m_Elements[elementName];
			if (element.m_Type != VertexStructureElement::Mat4Column)
				throw std::runtime_error("Invalid element type");

			if (elementIndex >= element.m_Count)
				throw std::runtime_error("Invalid element index");

			size_t offset = vertexIndex * m_Stride + element.m_Offset + elementIndex * sizeof(float) * 16;
			memcpy(&m_Data[offset], value.GetData(), sizeof(float) * 16);
		}

		VertexBuffer GenerateCompatible(const BufferLayout& layout) const
		{
			for (const auto& [name, element] : layout.GetStructure())
			{
				if (!m_Elements.ContainsKey(name))
					throw std::runtime_error("Incompatible buffer layout");

				if (m_Elements.Get(name) != element)
					throw std::runtime_error("Incompatible buffer layout");
			}

			VertexBuffer buffer(layout.GetStructure());
			buffer.m_Data.Resize((m_Data.Size() / m_Stride) * layout.GetStride());

			for (size_t i = 0; i < m_Data.Size() / m_Stride; i++)
			{
				for (const auto& [name, element] : layout.GetStructure())
				{
					const uint8_t* data = &m_Data[i * m_Stride + element.m_Offset];
					const size_t size = element.GetSize();
					memcpy(&buffer.m_Data[i * layout.GetStride() + element.m_Offset], data, size);
				}
			}

			return buffer;
		}

		const List<uint8_t>& GetData() const { return m_Data; }

	private:
		List<uint8_t> m_Data;
		size_t m_VertexCount = 0;
	};



	class VertexBufferBuilder
	{
	public:
		VertexBufferBuilder() = default;

		VertexBufferBuilder& AddElement(const String& name, VertexStructureElement::Type type, size_t count, bool normalized = false)
		{
			m_Elements[name] = VertexStructureElement(type, count, normalized);
			return *this;
		}

		VertexBuffer Build() const
		{
			return VertexBuffer(m_Elements);
		}

	private:
		HashMap<String, VertexStructureElement> m_Elements;
	};

	class InstanceBuffer : public BufferLayout
	{
	public:
		InstanceBuffer(HashMap<String, VertexStructureElement> elements)
			: BufferLayout(elements)
		{
			m_Data.Resize(m_Stride);
		}

		void SetElement(const String& elementName, float value, size_t elementIndex = 0)
		{
			if (!m_Elements.ContainsKey(elementName))
				throw std::runtime_error("Invalid element name");

			auto& element = m_Elements[elementName];
			if (element.m_Type != VertexStructureElement::Float)
				throw std::runtime_error("Invalid element type");

			if (elementIndex >= element.m_Count)
				throw std::runtime_error("Invalid element index");

			size_t offset = element.m_Offset + elementIndex * sizeof(float);
			*(float*)&m_Data[offset] = value;
		}

		// TODO: Vec2<float>
		// void SetElement(const String& elementName, Vec2<float> value, size_t elementIndex = 0)
		// {
		//     if(!m_Elements.ContainsKey(elementName))
		//         throw std::runtime_error("Invalid element name");
		//
		//     auto& element = m_Elements[elementName];
		//     if (element.m_Type != VertexStructureElement::Float2)
		//         throw std::runtime_error("Invalid element type");
		//
		//     if (elementIndex >= element.m_Count)
		//         throw std::runtime_error("Invalid element index");
		//
		//     size_t offset = element.m_Offset + elementIndex * sizeof(float) * 2;
		//     *(float*)&m_Data[offset + sizeof(float) * 0] = value.x;
		//     *(float*)&m_Data[offset + sizeof(float) * 1] = value.y;
		// }

		void SetElement(const String& elementName, Math::Vec3<float> value, size_t elementIndex = 0)
		{
		    if(!m_Elements.ContainsKey(elementName))
		        throw std::runtime_error("Invalid element name");
		
		    auto& element = m_Elements[elementName];
		    if (element.m_Type != VertexStructureElement::Float3)
		        throw std::runtime_error("Invalid element type");
		
		    if (elementIndex >= element.m_Count)
		        throw std::runtime_error("Invalid element index");
		
		    size_t offset = element.m_Offset + elementIndex * sizeof(float) * 3;
		    *(float*)&m_Data[offset + sizeof(float) * 0] = value.x;
		    *(float*)&m_Data[offset + sizeof(float) * 1] = value.y;
		    *(float*)&m_Data[offset + sizeof(float) * 2] = value.z;
		}

		void SetElement(const String& elementName, Math::Vec4<float> value, size_t elementIndex = 0)
		{
		    if(!m_Elements.ContainsKey(elementName))
		        throw std::runtime_error("Invalid element name");
		
		    auto& element = m_Elements[elementName];
		    if (element.m_Type != VertexStructureElement::Float4)
		        throw std::runtime_error("Invalid element type");
		
		    if (elementIndex >= element.m_Count)
		        throw std::runtime_error("Invalid element index");
		
		    size_t offset = element.m_Offset + elementIndex * sizeof(float) * 4;
		    *(float*)&m_Data[offset + sizeof(float) * 0] = value.x;
		    *(float*)&m_Data[offset + sizeof(float) * 1] = value.y;
		    *(float*)&m_Data[offset + sizeof(float) * 2] = value.z;
		    *(float*)&m_Data[offset + sizeof(float) * 3] = value.w;
		}

		void SetElement(const String& elementName, int value, size_t elementIndex = 0)
		{
			if (!m_Elements.ContainsKey(elementName))
				throw std::runtime_error("Invalid element name");

			auto& element = m_Elements[elementName];
			if (element.m_Type != VertexStructureElement::Int)
				throw std::runtime_error("Invalid element type");

			if (elementIndex >= element.m_Count)
				throw std::runtime_error("Invalid element index");

			size_t offset = element.m_Offset + elementIndex * sizeof(int);
			*(int*)&m_Data[offset] = value;
		}

		// TODO: Vec2<int>
		// void SetElement(const String& elementName, Vec2<int> value, size_t elementIndex = 0)
		// {
		//     if(!m_Elements.ContainsKey(elementName))
		//         throw std::runtime_error("Invalid element name");
		//
		//     auto& element = m_Elements[elementName];
		//     if (element.m_Type != VertexStructureElement::Int2)
		//         throw std::runtime_error("Invalid element type");
		//
		//     if (elementIndex >= element.m_Count)
		//         throw std::runtime_error("Invalid element index");
		//
		//     size_t offset = element.m_Offset + elementIndex * sizeof(int) * 2;
		//     *(int*)&m_Data[offset + sizeof(int) * 0] = value.x;
		//     *(int*)&m_Data[offset + sizeof(int) * 1] = value.y;
		// }

		void SetElement(const String& elementName, Math::Vec3<int> value, size_t elementIndex = 0)
		{
		    if(!m_Elements.ContainsKey(elementName))
		        throw std::runtime_error("Invalid element name");
		
		    auto& element = m_Elements[elementName];
		    if (element.m_Type != VertexStructureElement::Int3)
		        throw std::runtime_error("Invalid element type");
		
		    if (elementIndex >= element.m_Count)
		        throw std::runtime_error("Invalid element index");
		
		    size_t offset = element.m_Offset + elementIndex * sizeof(int) * 3;
		    *(int*)&m_Data[offset + sizeof(int) * 0] = value.x;
		    *(int*)&m_Data[offset + sizeof(int) * 1] = value.y;
		    *(int*)&m_Data[offset + sizeof(int) * 2] = value.z;
		}

		void SetElement(const String& elementName, Math::Vec4<int> value, size_t elementIndex = 0)
		{
		    if(!m_Elements.ContainsKey(elementName))
		        throw std::runtime_error("Invalid element name");
		
		    auto& element = m_Elements[elementName];
		    if (element.m_Type != VertexStructureElement::Int4)
		        throw std::runtime_error("Invalid element type");
		
		    if (elementIndex >= element.m_Count)
		        throw std::runtime_error("Invalid element index");
		
		    size_t offset = element.m_Offset + elementIndex * sizeof(int) * 4;
		    *(int*)&m_Data[offset + sizeof(int) * 0] = value.x;
		    *(int*)&m_Data[offset + sizeof(int) * 1] = value.y;
		    *(int*)&m_Data[offset + sizeof(int) * 2] = value.z;
		    *(int*)&m_Data[offset + sizeof(int) * 3] = value.w;
		}

		void SetElement(const String& elementName, bool value, size_t elementIndex = 0)
		{
			if (!m_Elements.ContainsKey(elementName))
				throw std::runtime_error("Invalid element name");

			auto& element = m_Elements[elementName];
			if (element.m_Type != VertexStructureElement::Bool)
				throw std::runtime_error("Invalid element type");

			if (elementIndex >= element.m_Count)
				throw std::runtime_error("Invalid element index");

			size_t offset = element.m_Offset + elementIndex * sizeof(bool);
			*(bool*)&m_Data[offset] = value;
		}

		void SetElement(const String& elementName, Math::Mat4<float, Math::ROW_MAJOR> value, size_t elementIndex = 0)
		{
		    if(!m_Elements.ContainsKey(elementName))
		        throw std::runtime_error("Invalid element name");
		
		    auto& element = m_Elements[elementName];
		    if (element.m_Type != VertexStructureElement::Mat4Row)
		        throw std::runtime_error("Invalid element type");
		
		    if (elementIndex >= element.m_Count)
		        throw std::runtime_error("Invalid element index");
		
		    size_t offset = element.m_Offset + elementIndex * sizeof(float) * 16;
			memcpy(&m_Data[offset], value.GetData(), sizeof(float) * 16);
		}

		void SetElement(const String& elementName, Math::Mat4<float, Math::COLUMN_MAJOR> value, size_t elementIndex = 0)
		{
			if (!m_Elements.ContainsKey(elementName))
				throw std::runtime_error("Invalid element name");

			auto& element = m_Elements[elementName];
			if (element.m_Type != VertexStructureElement::Mat4Column)
				throw std::runtime_error("Invalid element type");

			if (elementIndex >= element.m_Count)
				throw std::runtime_error("Invalid element index");

			size_t offset = element.m_Offset + elementIndex * sizeof(float) * 16;
			memcpy(&m_Data[offset], value.GetData(), sizeof(float) * 16);
		}

		InstanceBuffer GenerateCompatible(const BufferLayout& layout)
		{
			for (const auto& [name, element] : layout.GetStructure())
			{
				if (!m_Elements.ContainsKey(name))
					throw std::runtime_error("Incompatible buffer layout");

				if (m_Elements[name] != element)
					throw std::runtime_error("Incompatible buffer layout");
			}

			InstanceBuffer buffer(layout.GetStructure());
			buffer.m_Data.Resize(layout.GetStride());

			for (const auto& [name, element] : layout.GetStructure())
			{
				const uint8_t* data = &m_Data[element.m_Offset];
				const size_t size = element.GetSize();
				memcpy(&buffer.m_Data[element.m_Offset], data, size);
			}

			return buffer;
		}

		const List<uint8_t>& GetData() const { return m_Data; }

	private:
		List<uint8_t> m_Data;
	};

	class InstanceBufferBuilder
	{
	public:
		InstanceBufferBuilder() = default;

		InstanceBufferBuilder& AddElement(const String& name, VertexStructureElement::Type type, size_t count, bool normalized = false)
		{
			m_Elements[name] = VertexStructureElement(type, count, normalized);
			return *this;
		}

		InstanceBuffer Build() const
		{
			return InstanceBuffer(m_Elements);
		}

	private:
		HashMap<String, VertexStructureElement> m_Elements;
	};
}