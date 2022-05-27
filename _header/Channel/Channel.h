#pragma once

#include <vector>

#include "Utility/Void.h"

namespace wasp::channel {

	class ChannelBase{
	public:
		virtual ~ChannelBase() = 0;
	};

	template <typename T = utility::Void>
	class Channel : ChannelBase{
	private:
		std::vector<T> messages{};

	public:
		Channel() = default;

		const auto& getMessages() const {
			return messages;
		}

		void addMessage(const T& message) {
			messages.push_back(message);
		}

		template <typename... Ts>
		void emplaceMessage(Ts&&... args) {
			messages.emplace_back(args...);
		}

		void clear() {
			messages.clear();
		}
	};

	template<>
	class Channel<utility::Void> : ChannelBase {
	private:
		std::size_t messages{};

	public:
		Channel()
			: messages{ 0 } {
		}

		std::size_t getMessages() const {
			return messages;
		}

		void addMessage() {
			++messages;
		}

		void clear() {
			messages = 0;
		}
	};
}