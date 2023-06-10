#pragma once
namespace dae
{
	template <typename T>
	class Singleton
	{
	public:
		static T& GetInstance()
		{
			static T instance{};
			return instance;
		}

		virtual ~Singleton() { m_IsDestroyed = true; }
		Singleton(const Singleton& other) = delete;
		Singleton(Singleton&& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;
		Singleton& operator=(Singleton&& other) = delete;

		static bool GetIsDestroyed() { return m_IsDestroyed; }

	protected:
		Singleton() = default;

	private:
		static bool m_IsDestroyed;
	};

	template <typename T>
	bool Singleton<T>::m_IsDestroyed = false;
}