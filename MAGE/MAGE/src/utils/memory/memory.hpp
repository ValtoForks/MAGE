#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "utils\platform\windows.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <functional>
#include <memory>
#include <wrl.h>

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Defines
//-----------------------------------------------------------------------------
#pragma region

// Pointer size macros
#if defined(__amd64__) || defined(_M_X64)
	#define MAGE_X64
	#define MAGE_POINTER_SIZE 8
#elif defined(__i386__) || defined(_M_IX86)
	#define MAGE_X86
	#define MAGE_POINTER_SIZE 4
#endif

// Memory management macros
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p) = nullptr; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p) = nullptr; } }

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// ComPtr
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 A class of smart pointers for managing shared-ownership COM resources.

	 @tparam		T
					The COM resource type.
	 */
	template < typename T >
	using ComPtr = Microsoft::WRL::ComPtr< T >;

	#pragma endregion

	//-------------------------------------------------------------------------
	// SharedPtr
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 A class of smart pointers for managing shared-ownership memory resources.

	 @tparam		T
					The memory resource type.
	 */
	template < typename T >
	using SharedPtr = std::shared_ptr< T >;

	/**
	 Constructs an object of type T. 

	 No custom allocator is used for constructing the object of type T.

	 @tparam		T
					The type.
	 @tparam		ConstructorArgsT
					The constructor argument types.
	 @return		A shared pointer to the constructed object of type T.
	 */
	template< typename T, typename... ConstructorArgsT >
	SharedPtr< T > MakeShared(ConstructorArgsT&&... args);

	/**
	 Constructs an object of type T.

	 The custom allocator is used for constructing the object of type T.

	 @tparam		T
					The type.
	 @tparam		ConstructorArgsT
					The constructor argument types.
	 @return		A shared pointer to the constructed object of type T.
	 */
	template< typename T, typename... ConstructorArgsT >
	SharedPtr< T > MakeAllocatedShared(ConstructorArgsT&&... args);

	#pragma endregion

	//-------------------------------------------------------------------------
	// WeakPtr
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 A class of smart pointers for referencing but not managing 
	 shared-ownership memory resources.

	 @tparam		T
					The memory resource type.
	 */
	template < typename T >
	using WeakPtr = std::weak_ptr< T >;

	#pragma endregion

	//-------------------------------------------------------------------------
	// UniquePtr
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 A class of smart pointers for managing exclusive-ownership memory 
	 resources.

	 @tparam		T
					The memory resource type.
	 */
	template < typename T, typename DeleterT = std::default_delete< T > >
	using UniquePtr = std::unique_ptr< T, DeleterT >;

	/**
	 Constructs an object of type T.

	 @tparam		T
					The type.
	 @tparam		ConstructorArgsT
					The constructor argument types.
	 @return		A unique pointer to the constructed object of type T.
	 */
	template< typename T, typename... ConstructorArgsT >
	UniquePtr< T > MakeUnique(ConstructorArgsT&&... args);
	
	/**
	 Creates a unique pointer whose stored pointer is obtained by statically 
	 casting the stored pointer of the given unique pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the unique pointer to cast.
	 @return		The moved unique pointer.
	 */
	template< typename ToT, typename FromT >
	UniquePtr< ToT > static_pointer_cast(UniquePtr< FromT > &&ptr) noexcept;

	/**
	 Creates a unique pointer whose stored pointer is obtained by dynamically 
	 casting the stored pointer of the given unique pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the unique pointer to cast.
	 @return		The moved unique pointer.
	 */
	template< typename ToT, typename FromT >
	UniquePtr< ToT > dynamic_pointer_cast(UniquePtr< FromT > &&ptr) noexcept;

	/**
	 Creates a unique pointer whose stored pointer is obtained by const 
	 casting the stored pointer of the given unique pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the unique pointer to cast.
	 @return		The moved unique pointer.
	 */
	template< typename ToT, typename FromT >
	UniquePtr< ToT > const_pointer_cast(UniquePtr< FromT > &&ptr) noexcept;

	/**
	 Creates a unique pointer whose stored pointer is obtained by reinterpret 
	 casting the stored pointer of the given unique pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the unique pointer to cast.
	 @return		The moved unique pointer.
	 */
	template< typename ToT, typename FromT >
	UniquePtr< ToT > reinterpret_pointer_cast(UniquePtr< FromT > &&ptr) noexcept;

	#pragma endregion

	//-------------------------------------------------------------------------
	// UniqueHandle and SharedHandle
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 Destructs the given handle.

	 @param[in]		handle
					The handle to destruct.
	 */
	static inline void DestructHandle(HANDLE handle) noexcept {
		if (handle) {
			CloseHandle(handle);
		}
	}

	/**
	 A struct of handle destructors (i.e. for closing handles).
	 */
	struct HandleCloser final {

		/**
		 Destructs the given handle.

		 @param[in]		handle
						The handle to destruct.
		 */
		void operator()(HANDLE handle) const noexcept {
			DestructHandle(handle);
		}
	};

	/**
	 A class of smart pointers for managing exclusive-ownership handle 
	 resources.
	 */
	using UniqueHandle = UniquePtr< void, HandleCloser >;

	/**
	 A class of smart pointers for managing shared-ownership handle resources.
	 */
	using SharedHandle = SharedPtr< void >;

	/**
	 Converts the given handle to a safe handle.

	 @param[in]		handle
					A handle.
	 @return		If the given handle is an invalid handle,
					@c nullptr is returned.
	 @return		Otherwise, the given handle is returned.

	 */
	static inline HANDLE SafeHandle(HANDLE handle) noexcept {
		return (handle == INVALID_HANDLE_VALUE) ? nullptr : handle;
	}

	/**
	 Creates a unique handle for the given handle.

	 @param[in]		handle
					A handle.
	 @return		A unique handle for the given handle @a handle.
	 */
	inline UniqueHandle CreateUniqueHandle(HANDLE handle) {
		return UniqueHandle(SafeHandle(handle));
	}

	/**
	 Creates a shared handle for the given handle.

	 @param[in]		handle
					A handle.
	 @return		A shared handle for the given handle @a handle.
	 */
	inline SharedHandle CreateSharedHandle(HANDLE handle) {
		return SharedHandle(SafeHandle(handle), DestructHandle);
	}

	#pragma endregion

	//-------------------------------------------------------------------------
	// UniqueFileStream
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 A struct of file stream destructors (i.e. for closing file streams).
	 */
	struct FileStreamCloser final {

		/**
		 Destructs the file stream.

		 @param[in]		stream
						A pointer to a file stream to destruct.
		 */
		void operator()(FILE *stream) const noexcept {
			if (stream) {
				fclose(stream);
			}
		}
	};

	/**
	 A class of smart pointers for managing exclusive-ownership file streams.
	 */
	using UniqueFileStream = UniquePtr< FILE, FileStreamCloser >;

	#pragma endregion

	//-------------------------------------------------------------------------
	// ProxyPtr
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 A class of proxy pointers.

	 @tparam		T
					The memory resource type.	
	 */
	template< typename T >
	class ProxyPtr final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a proxy pointer.
		 */
		ProxyPtr() noexcept
			: ProxyPtr([]() noexcept -> T * {
				return nullptr;
			}) {}

		/**
		 Constructs a proxy pointer.
		 */
		ProxyPtr(std::nullptr_t) noexcept
			: ProxyPtr() {}

		/**
		 Constructs a proxy pointer for the given container and index.

		 @tparam		ContainerT
						The container type.
		 @param[in]		container
						A reference to the container.
		 @param[in]		index
						The index into the container.
		 */
		template< typename ContainerT >
		explicit ProxyPtr(ContainerT &container, size_t index) noexcept
			: ProxyPtr([&container, index]() noexcept {
				return &container[index];
			}) {}
			
		/**
		 Constructs a proxy pointer with the given getter function.

		 @param[in]		getter
						The getter function.
		 */
		explicit ProxyPtr(std::function< T *() > getter) noexcept
			: m_getter(std::move(getter)) {}

		/**
		 Constructs a proxy pointer from the given proxy pointer.

		 @param[in]		ptr
						A reference to the proxy pointer.
		 */
		ProxyPtr(const ProxyPtr &ptr) noexcept
			: m_getter(ptr.m_getter) {}
		
		/**
		 Constructs a proxy pointer by moving the given proxy pointer.

		 @param[in]		ptr
						A reference to the proxy pointer to move.
		 */
		ProxyPtr(ProxyPtr &&ptr) noexcept
			: m_getter(std::move(ptr.m_getter)) {}

		/**
		 Constructs a proxy pointer from the given proxy pointer.

		 @tparam		U
						The memory resource type.
		 @param[in]		ptr
						A reference to the proxy pointer.
		 */
		template< typename U >
		ProxyPtr(const ProxyPtr< U > &ptr) noexcept;

		/**
		 Constructs a proxy pointer by moving the given proxy pointer.

		 @tparam		U
						The memory resource type.
		 @param[in]		ptr
						A reference to the proxy pointer to move.
		 */
		template< typename U >
		ProxyPtr(ProxyPtr< U > &&ptr) noexcept;
	
		/**
		 Destructs this proxy pointer.
		 */
		~ProxyPtr() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given proxy pointer to this proxy pointer.

		 @param[in]		ptr
						A reference to the proxy pointer to copy.
		 @return		A reference to the copy of the given proxy pointer 
						(i.e. this proxy pointer).
		 */
		ProxyPtr &operator=(const ProxyPtr &ptr) noexcept {
			m_getter = ptr.m_getter;
			return *this;
		}
		
		/**
		 Moves the given proxy pointer to this proxy pointer.

		 @param[in]		ptr
						A reference to the proxy pointer to move.
		 @return		A reference to the moved proxy pointer (i.e. this proxy 
						pointer).
		 */
		ProxyPtr &operator=(ProxyPtr &&ptr) noexcept {
			m_getter = std::move(ptr.m_getter);
			return *this;
		}

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Converts this proxy pointer to a @c bool.		
		 */
		explicit operator bool() const noexcept {
			return nullptr != Get();
		}

		/**
		 Returns the memory resource pointed to by this proxy pointer.

		 @return		A reference to the memory resource pointed to by this 
						proxy pointer.
		 */
		T &operator*() const noexcept {
			return *Get();
		}

		/**
		 Returns the memory resource pointed to by this proxy pointer.

		 @return		A pointer to the memory resource pointed to by this 
						proxy pointer.
		 */
		T *operator->() const noexcept {
			return Get();
		}

		/**
		 Returns the memory resource pointed to by this proxy pointer.

		 @return		A pointer to the memory resource pointed to by this 
						proxy pointer.
		 */
		T *Get() const noexcept {
			return m_getter();
		}

		/**
		 Checks whether the given proxy pointer is equal to this proxy pointer.

		 @tparam		U
						The memory resource type.
		 @param[in]		rhs
						A reference to the proxy pointer.
		 @return		@c true if the given proxy pointer is equal to this 
						proxy pointer. @c false otherwise.
		 */
		template< typename U >
		bool operator==(const ProxyPtr< U >& rhs) const noexcept { 
			return Get() == rhs.Get(); 
		}
		
		/**
		 Checks whether the given proxy pointer is not equal to this proxy 
		 pointer.

		 @tparam		U
						The memory resource type.
		 @param[in]		rhs
						A reference to the proxy pointer.
		 @return		@c true if the given proxy pointer is not equal to this 
						proxy pointer. @c false otherwise.
		 */
		template< typename U >
		bool operator!=(const ProxyPtr< U >& rhs) const noexcept {
			return !(*this == rhs);
		}

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The getter of this proxy pointer. 
		 */
		std::function< T *() > m_getter;
	};

	/**
	 Checks whether the given proxy pointer is equal to @c nullptr.

	 @tparam		T
					The memory resource type.
	 @param[in]		lhs
					A reference to the proxy pointer.
	 @return		@c true if the given proxy pointer is equal to @c nullptr.
					@c false otherwise.
	 */
	template< typename T >
	inline bool operator==(const ProxyPtr< T >& lhs,
		                   std::nullptr_t) noexcept {
		
		return !bool(lhs);
	}

	/**
	 Checks whether the given proxy pointer is not equal to @c nullptr.

	 @tparam		T
					The memory resource type.
	 @param[in]		lhs
					A reference to the proxy pointer.
	 @return		@c true if the given proxy pointer is equal to @c nullptr.
					@c false otherwise.
	 */
	template< typename T >
	inline bool operator!=(const ProxyPtr< T >& lhs,
		                   std::nullptr_t) noexcept {
		
		return bool(lhs);
	}

	/**
	 Checks whether the given proxy pointer is equal to @c nullptr.

	 @tparam		T
					The memory resource type.
	 @param[in]		rhs
					A reference to the proxy pointer.
	 @return		@c true if the given proxy pointer is equal to @c nullptr.
					@c false otherwise.
	 */
	template< typename T >
	inline bool operator==(std::nullptr_t,
		                   const ProxyPtr< T >& rhs) noexcept {
		
		return !bool(rhs);
	}

	/**
	 Checks whether the given proxy pointer is not equal to @c nullptr.

	 @tparam		T
					The memory resource type.
	 @param[in]		rhs
					A reference to the proxy pointer.
	 @return		@c true if the given proxy pointer is equal to @c nullptr.
					@c false otherwise.
	 */
	template< typename T >
	inline bool operator!=(std::nullptr_t,
		                   const ProxyPtr< T >& rhs) noexcept {
		
		return bool(rhs);
	}

	/**
	 Creates a proxy pointer whose stored getter is obtained by statically 
	 casting the stored getter of the given proxy pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the proxy pointer to cast.
	 @return		The moved proxy pointer.
	 */
	template< typename ToT, typename FromT >
	ProxyPtr< ToT > static_pointer_cast(const ProxyPtr< FromT > &ptr) noexcept;

	/**
	 Creates a proxy pointer whose stored getter is obtained by statically 
	 casting the stored getter of the given proxy pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the proxy pointer to cast.
	 @return		The moved proxy pointer.
	 */
	template< typename ToT, typename FromT >
	ProxyPtr< ToT > static_pointer_cast(const ProxyPtr< FromT > &ptr) noexcept;

	/**
	 Creates a proxy pointer whose stored getter is obtained by dynamically 
	 casting the stored getter of the given proxy pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the proxy pointer to cast.
	 @return		The moved proxy pointer.
	 */
	template< typename ToT, typename FromT >
	ProxyPtr< ToT > dynamic_pointer_cast(const ProxyPtr< FromT > &ptr) noexcept;

	/**
	 Creates a proxy pointer whose stored getter is obtained by dynamically 
	 casting the stored getter of the given proxy pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the proxy pointer to cast.
	 @return		The moved proxy pointer.
	 */
	template< typename ToT, typename FromT >
	ProxyPtr< ToT > dynamic_pointer_cast(ProxyPtr< FromT > &&ptr) noexcept;

	/**
	 Creates a proxy pointer whose stored getter is obtained by const 
	 casting the stored getter of the given proxy pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the proxy pointer to cast.
	 @return		The moved proxy pointer.
	 */
	template< typename ToT, typename FromT >
	ProxyPtr< ToT > const_pointer_cast(const ProxyPtr< FromT > &ptr) noexcept;

	/**
	 Creates a proxy pointer whose stored getter is obtained by const 
	 casting the stored getter of the given proxy pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the proxy pointer to cast.
	 @return		The moved proxy pointer.
	 */
	template< typename ToT, typename FromT >
	ProxyPtr< ToT > const_pointer_cast(ProxyPtr< FromT > &&ptr) noexcept;

	/**
	 Creates a proxy pointer whose stored getter is obtained by reinterpret 
	 casting the stored getter of the given proxy pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the proxy pointer to cast.
	 @return		The moved proxy pointer.
	 */
	template< typename ToT, typename FromT >
	ProxyPtr< ToT > reinterpret_pointer_cast(const ProxyPtr< FromT > &ptr) noexcept;

	/**
	 Creates a proxy pointer whose stored getter is obtained by reinterpret 
	 casting the stored getter of the given proxy pointer.

	 @tparam		ToT
					The conversion to-type.
	 @tparam		FromT
					The conversion from-type.
	 @param[in]		ptr
					A reference to the proxy pointer to cast.
	 @return		The moved proxy pointer.
	 */
	template< typename ToT, typename FromT >
	ProxyPtr< ToT > reinterpret_pointer_cast(ProxyPtr< FromT > &&ptr) noexcept;

	#pragma endregion
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "utils\memory\memory.tpp"

#pragma endregion