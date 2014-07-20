#ifndef EUDAQ_INCLUDED_ExampleHardware
#define EUDAQ_INCLUDED_ExampleHardware

#include "eudaq/Timer.hh"
#include <vector>

namespace eudaq {

  class DLLEXPORT ExampleHardware {
    public:
      ExampleHardware();
      void Setup(int);
      void PrepareForRun();
      bool EventsPending() const;
      unsigned NumSensors() const;
      std::vector<unsigned char> ReadSensor(int sensorid);
      void CompletedEvent(); 
      void SetHeight(unsigned short v) {m_height = v;}
      void SetWidth(unsigned short u) {m_width = u;}
      void SetThreshold(unsigned short thr) {m_threshold = thr;}
      void SetParticles(unsigned short value) {m_particles = value;}

    private:
      unsigned short m_numsensors, m_width, m_height, m_triggerid;
      unsigned short m_threshold, m_particles;
      eudaq::Timer m_timer;
      double m_nextevent;
  };

} // namespace eudaq

#endif // EUDAQ_INCLUDED_ExampleHardware
