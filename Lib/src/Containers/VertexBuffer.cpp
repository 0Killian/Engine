#include "Containers/VertexBuffer.h"

namespace NGN
{
	InstanceBuffer InstanceBuffer::Merge(const InstanceBuffer& other)
	{
		InstanceBufferBuilder builder;
		List<uint8_t> data;

		for (const auto& [name, element] : GetStructure())
		{
			builder.AddElement(name, element.m_Type, element.m_Count, element.m_Normalized);
			data.Resize(data.Size() + element.GetSize());
			memcpy(&data[data.Size() - element.GetSize()], &m_Data[element.m_Offset], element.GetSize());
		}

		for (const auto& [name, element] : other.GetStructure())
		{
			builder.AddElement(name, element.m_Type, element.m_Count, element.m_Normalized);
			data.Resize(data.Size() + element.GetSize());
			memcpy(&data[data.Size() - element.GetSize()], &other.m_Data[element.m_Offset], element.GetSize());
		}

		InstanceBuffer buffer = builder.Build();
		buffer.m_Data = data;

		return buffer;
	}
}