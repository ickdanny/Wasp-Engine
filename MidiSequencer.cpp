#include "MidiSequencer.h"

#include <stdexcept>
#include <thread>
#include <chrono>

namespace wasp::sound::midi {



/*#pragma pack(push, 1)
	struct _mid_track {
		unsigned int	id;	// identifier "MTrk"
		unsigned int	length;	// track length, big-endian
	};
#pragma pack(pop)
*/

	bool hundredNanosecondSleep(LONGLONG nanoseconds);

	MidiSequencer::MidiSequencer() {
		auto result{ midiOutOpen(&midiOutHandle, MIDI_MAPPER, 0, 0, CALLBACK_NULL) };
		if (result != MMSYSERR_NOERROR) {
			throw std::runtime_error{ "Error opening MIDI Mapper" };
		}
	}
	
	MidiSequencer::~MidiSequencer(){
		midiOutClose(midiOutHandle);
	}

	/*
	static unsigned long read_var_long(unsigned char* buf, unsigned int* bytesread)
	{
		unsigned long var = 0;
		unsigned char c;
		*bytesread = 0;
		do
		{
			c = buf[(*bytesread)++];
			var = (var << 7) + (c & 0x7f);
		} while (c & 0x80);
		return var;
	}

	static unsigned int get_buffer_ex6(unsigned char* buf, unsigned int len, unsigned int** out, unsigned int* outlen)
	{
		unsigned char* tmp;
		unsigned int* streambuf;
		unsigned int streamlen = 0;
		unsigned int bytesread;
		unsigned int buflen = 64;
		tmp = buf;
		*out = NULL;
		*outlen = 0;
		streambuf = (unsigned int*)malloc(sizeof(unsigned int) * buflen);
		if (streambuf == NULL)
			return 0;
		memset(streambuf, 0, sizeof(unsigned int) * buflen);
		tmp += sizeof(struct _mid_header);
		tmp += sizeof(struct _mid_track);
		while (tmp < buf + len)
		{
			unsigned char cmd;
			unsigned int time = read_var_long(tmp, &bytesread);
			unsigned int msg = 0;
			tmp += bytesread;
			cmd = *tmp++;
			if ((cmd & 0xf0) != 0xf0) // normal command
			{
				msg = ((unsigned long)cmd) |
					((unsigned long)*tmp++ << 8);
				if (!((cmd & 0xf0) == 0xc0 || (cmd & 0xf0) == 0xd0))
					msg |= ((unsigned long)*tmp++ << 16);
				streambuf[streamlen++] = time;
				streambuf[streamlen++] = msg;
			}
			else if (cmd == 0xff)
			{
				cmd = *tmp++; // cmd should be meta-event (0x2f for end of track)
				cmd = *tmp++; // cmd should be meta-event length
				tmp += cmd;
			}
		}
		*out = streambuf;
		*outlen = streamlen;
		return 0;
	}
	*/

	void MidiSequencer::test() {
		/*unsigned int* streambuf = NULL;
		unsigned int streamlen = 0;
		unsigned char* midibuf = NULL;
		unsigned int midilen = 0;
		unsigned int err;
		const unsigned int PPQN_CLOCK = 5;
		unsigned int i;

		midibuf = load_file((unsigned char*)"example6.mid", &midilen);
		if (midibuf == NULL)
		{
			printf("could not open example6.mid\n");
			return;
		}
		get_buffer_ex6(midibuf, midilen, &streambuf, &streamlen);
		i = 0;
		while (i < streamlen)
		{
			unsigned int time = streambuf[i++];
			Sleep(time * PPQN_CLOCK);
			err = midiOutShortMsg(midiOutHandle, streambuf[i++]);
			if (err != MMSYSERR_NOERROR)
				printf("error sending command: %d\n", err);
		}
		midiOutClose(midiOutHandle);
		free(streambuf);
		free(midibuf);
		*/
	}

	//https://gist.github.com/Youka/4153f12cf2e17a77314c
	bool hundredNanosecondSleep(LONGLONG hundredNanoseconds) {

		HANDLE timerHandle{ CreateWaitableTimer(NULL, TRUE, NULL) };
		if (!timerHandle) {
			return false;
		}

		LARGE_INTEGER time{};
		time.QuadPart = -hundredNanoseconds;

		if (!SetWaitableTimer(timerHandle, &time, 0, NULL, NULL, FALSE)) {
			CloseHandle(timerHandle);
			return FALSE;
		}
		/* Start & wait for timer */
		WaitForSingleObject(timerHandle, INFINITE);
		/* Clean resources */
		CloseHandle(timerHandle);
		/* Slept without problems */
		return TRUE;
	}
}