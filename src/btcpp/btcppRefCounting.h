#pragma once

#include <memory>

#ifdef BTCPP_PLATFORM_WINDOWS
#include <intrin.h>
#endif

class btcppRefCounted
{
public:
	inline uint32_t IncrementReferenceCount() const
	{
#ifdef BTCPP_PLATFORM_WINDOWS
		return static_cast<uint32_t>(::_InterlockedIncrement(reinterpret_cast<long*>(&m_RefCount)));
#else
		m_RefCount++;
		return m_RefCount;
#endif
	}

	inline uint32_t DecrementReferenceCount() const
	{
#ifdef BTCPP_PLATFORM_WINDOWS
		return static_cast<uint32_t>(::_InterlockedDecrement(reinterpret_cast<long*>(&m_RefCount)));
#else
		m_RefCount--;
		return m_RefCount;
#endif
	}

	uint32_t GetReferenceCount() const
	{
		return static_cast<uint32_t>(m_RefCount);
	}
private:
	mutable int32_t m_RefCount = 0;
};

template<typename ReferencedType>
class btcppRef
{
public:
	btcppRef()
		: m_Reference(nullptr)
	{
	}

	btcppRef(std::nullptr_t)
		: m_Reference(nullptr)
	{
	}

	btcppRef(ReferencedType* reference)
		: m_Reference(reference)
	{
		IncrementReferenceCount();
	}

	btcppRef(const btcppRef<ReferencedType>& other)
		: m_Reference(other.m_Reference)
	{
		IncrementReferenceCount();
	}

	btcppRef(const btcppRef<ReferencedType>&& other)
	{
		m_Reference = other.m_Reference;
		other->m_Reference = nullptr;
	}

	template<typename TOther>
	btcppRef(const btcppRef<TOther>& other)
	{
		m_Reference = static_cast<ReferencedType*>(other.m_Reference);
		IncrementReferenceCount();
	}

	template<typename TOther>
	btcppRef(btcppRef<TOther>&& other)
	{
		m_Reference = static_cast<ReferencedType*>(other.m_Reference);
		other.m_Reference = nullptr;
	}

	~btcppRef()
	{
		DecrementReferenceCount();
	}

	btcppRef& operator=(std::nullptr_t)
	{
		DecrementReferenceCount();
		m_Reference = nullptr;
		return *this;
	}

	btcppRef& operator=(const btcppRef<ReferencedType>& other)
	{
		other.IncrementReferenceCount();
		DecrementReferenceCount();
		m_Reference = other.m_Reference;
		return *this;
	}

	btcppRef& operator=(const btcppRef<ReferencedType>&& other)
	{
		DecrementReferenceCount();
		m_Reference = other.m_Reference;
		other.m_Reference = nullptr;
		return *this;
	}

	template<typename TOther>
	btcppRef& operator=(const btcppRef<TOther>& other)
	{
		other.IncrementReferenceCount();
		DecrementReferenceCount();
		m_Reference = other.m_Reference;
		return *this;
	}

	template<typename TOther>
	btcppRef& operator=(const btcppRef<TOther>&& other)
	{
		DecrementReferenceCount();
		m_Reference = other.m_Reference;
		other.m_Reference = nullptr;
		return *this;
	}

	operator bool() { return m_Reference != nullptr; }
	operator bool() const { return m_Reference != nullptr; }

	ReferencedType* operator->() { return m_Reference; }
	const ReferencedType* operator->() const { return m_Reference; }

	ReferencedType& operator*() { return *m_Reference; }
	const ReferencedType& operator*() const { return *m_Reference; }

	ReferencedType* Get() { return m_Reference; }
	const ReferencedType* Get() const { return m_Reference; }

	template<typename TOther>
	btcppRef<TOther> As() const
	{
		return btcppRef<TOther>(*this);
	}

	template<typename... TArgs>
	static btcppRef<ReferencedType> Create(TArgs&&... args)
	{
		return btcppRef<ReferencedType>(new ReferencedType(std::forward<TArgs>(args)...));
	}
private:
	void IncrementReferenceCount() const
	{
		if (m_Reference)
			m_Reference->IncrementReferenceCount();
	}

	void DecrementReferenceCount() const
	{
		if (!m_Reference)
			return;

		m_Reference->DecrementReferenceCount();

		if (m_Reference->GetReferenceCount() == 0)
		{
			delete m_Reference;
			m_Reference = nullptr;
		}
	}
private:
	mutable ReferencedType* m_Reference;

	template<class TOther>
	friend class btcppRef;
};

template<typename ReferencedType>
using btcppUnique = std::unique_ptr<ReferencedType>;

template<typename ReferencedType>
using btcppShared = std::shared_ptr<ReferencedType>;

template<typename ReferencedType, typename... TArgs>
constexpr btcppUnique<ReferencedType> CreateUnique(TArgs&&... args)
{
	return std::make_unique<ReferencedType>(std::forward<TArgs>(args)...);
}

template<typename ReferencedType, typename... TArgs>
constexpr btcppShared<ReferencedType> CreateShared(TArgs&&... args)
{
	return std::make_shared<ReferencedType>(std::forward<TArgs>(args)...);
}