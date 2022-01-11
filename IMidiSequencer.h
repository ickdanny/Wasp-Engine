#pragma once

namespace wasp::sound::midi {
	class IMidiSequencer {
	public:
		static constexpr int infiniteLoop{ -1 };

		virtual void setLoopCount(int loopCount) = 0;
		virtual void setLoopEndPoint(int endPoint) = 0;
		virtual void setLoopStartPoint(int startPoint) = 0;
		virtual void stop() = 0;
		virtual void start() = 0;
		virtual void setSequence() = 0; //todo: what do we pass into this
		virtual bool isRunning() = 0;
	};
}