#pragma once

#include <vector>
#include <memory>

#include "Channel.h"
#include "Topic.h"

namespace wasp::channel {

	class ChannelSet {
	private:
		//fields
		std::vector<std::unique_ptr<ChannelBase>> channels{};

	public:
		//default constructor
		ChannelSet() = default;

		template <typename T>
		Channel<T>& getChannel(const Topic<T>& topic) {
			//resize the pointer vector if necessary
			if (topic.index >= channels.size()) {
				channels.resize(topic.index + 1);
			}
			auto& channelPointer{ channels[topic.index] };
			//if stored pointer is not nullptr, return the result of dereference
			if (channelPointer) {
				return static_cast<Channel<T>&>(*channelPointer);
			}
			//otherwise, make a new channel and return that
			else {
				channels[topic.index] = std::make_unique<Channel<T>>();
				return static_cast<Channel<T>&>(*channels[topic.index]);
			}
		}

		void clear() {
			channels.clear();
		}
	};
}