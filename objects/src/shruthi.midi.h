#ifndef MutableSysex_shruthi_midi_h
#define MutableSysex_shruthi_midi_h


#ifdef _DEBUG
#define __RTMIDI_DEBUG__ 0
#else
#define __RTMIDI_DEBUG__ 0
#endif

#include "ext.h"
#include "maxcpp6.h"
#include "rtmidi/RtMidi.h"
#include "shruthi.types.h"
#include "shruthi.transfer.h"

class RtMidiIn;
class RtMidiOut;
class VxShruthi;

class SysexBulkTransfer;

class ShruthiMidi {
 public:
  ShruthiMidi(VxShruthi &x);
  ~ShruthiMidi();

  void parseSysex(const std::vector<uint8_t> &msg);
  void parseControlChange(long cc_index, long cc_value);

  int expectedSysexPayload(SysexCommand cmd);
  bool validateSysexChecksum(const std::vector<uint8_t> &msg, uint8_t checksum);
  void sendSysex(uint8_t *data, uint8_t command, uint8_t argument, size_t size);
  void sendSysexCommand(uint8_t command, uint8_t argument = 0);

  void sendSequenceProgramChange(long slot);
  void sendPatchProgramChange(long slot);

  void stopTransfer() { transfer_.stop(); }

  void clearCache() { lastNrpnIndex_ = -1; }

  void sendMessage(const std::vector<uint8_t> &msg);
  void sendNrpn(long nrpn_index, long nrpn_value);

  void setOutputChannel(long v) {
    channelOut_ = CLAMP(v - 1, 0, 15);  // 0 en 1 zijn beiden 1 op shruthi
    DPOST("Set output channel %d", v);
  }

  void testMidiOut();
  bool isValidSysex(const std::vector<uint8_t> &sysex);

  void setOutlets(void *midiOutlet, void *msgOutlet) {
    midiOutlet_ = midiOutlet;
    msgOutlet_ = msgOutlet;
  }

 private:
  void parseControlChangeAsNrpn();
  t_atom atoms_[140];  // sysex dump blocksize is 128

  VxShruthi &x_;  // dirty hack

  SysexBulkTransfer transfer_;

  RtMidiIn *midiInput_;
  RtMidiIn *midiAuxInput_;
  RtMidiOut *midiOutput_;

  uint8_t channelOut_;

  uint8_t nrpnMsb_;
  uint8_t nrpnLsb_;
  uint8_t dataMsb_;
  uint8_t dataLsb_;
  uint8_t lastDataMsb_;

  long lastNrpnIndex_;

  long indexLsb_;
  long indexMsb_;
  long valueLsb_;
  long valueMsb_;
  bool isNrpnValid_;

  void *midiOutlet_;
  void *msgOutlet_;
};

#endif
