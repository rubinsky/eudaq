#include "eudaq/ExampleHardware.hh"
#include "eudaq/Utils.hh"
#include <random>
#include <iostream>

namespace eudaq {

  namespace {
    std::default_random_engine generator_poison;
    std::default_random_engine generator;
    std::default_random_engine generator_x;
    std::default_random_engine generator_y;

    std::vector<unsigned char> MakeRawEvent(unsigned width, unsigned height) {
      std::vector<unsigned char> result(width*height*2);
      size_t offset = 0;

      std::poisson_distribution<int> distribution(1.0);

      for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
          short charge = distribution(generator);

          setlittleendian(&result[offset], charge);
          offset += 2;
        }
      }
      return result;
    }

    std::vector<unsigned char> ZeroSuppressEvent( unsigned width, unsigned height, unsigned threshold, unsigned particles ) {
      std::poisson_distribution<unsigned short> distribution_particles( static_cast<float> (particles) );
      unsigned short l_particles = distribution_particles( generator_poison );

      std::normal_distribution<double> distribution_x( width/2., width/6.);

      std::normal_distribution<double> distribution_y( height/2., height/6.);


      std::vector<unsigned char> result;
 
//      std::cout << " generating " << width << " x " << height << " with " << l_particles << " particles " << " mean:<"<< particles << ">" << std::endl; 
 
      size_t inoffset = 0, outoffset = 0;
      for( unsigned i_particle =0; i_particle< l_particles; i_particle ++ ) {
        
        double x = distribution_x( generator_x);
        double y = distribution_y( generator_y); 
        if( x < 0. || x > width || y < 0. || y > height ) continue;
//        std::cout << " generating pixels at  " << x << " " << y << std::endl; 
            result.resize(outoffset+6);
            setlittleendian<unsigned short>(&result[outoffset+0], static_cast<int> (x) );
            setlittleendian<unsigned short>(&result[outoffset+2], static_cast<int> (y) );
            setlittleendian<short>(&result[outoffset+4], 0);
            outoffset += 6;       
      }

      return result;
    }

  }

  ExampleHardware::ExampleHardware()
    : m_numsensors(6),
    m_width(1152),
    m_height(576),
    m_triggerid(0),
    m_threshold(10),
    m_particles(1)
  {}

  void ExampleHardware::Setup(int) {
  }

  void ExampleHardware::PrepareForRun() {
    m_timer.Restart();
    m_nextevent = 10.0;
    m_triggerid = 0;
  }

  bool ExampleHardware::EventsPending() const {
    return m_timer.Seconds() > m_nextevent;
  }

  unsigned ExampleHardware::NumSensors() const {
    return m_numsensors;
  }

  std::vector<unsigned char> ExampleHardware::ReadSensor(int sensorid) {
    std::vector<unsigned char> result(8);
    setlittleendian<unsigned short>(&result[0], m_width);
    setlittleendian<unsigned short>(&result[2], m_height);
//    if (sensorid % 2 == 0) {
//      // Raw Data
//      setlittleendian(&result[6], static_cast<unsigned short>(0));
//      std::vector<unsigned char> data = MakeRawEvent(m_width, m_height);
//      result.insert(result.end(), data.begin(), data.end());
//    } else {
      // Zero suppressed data
      std::vector<unsigned char> data = ZeroSuppressEvent( m_width, m_height, m_threshold, m_particles);
      result.insert(result.end(), data.begin(), data.end());
      unsigned short numhits = (result.size() - 8) / 6;
      setlittleendian<unsigned short>(&result[4], 0x8000 | numhits);
      setlittleendian<unsigned short>(&result[6], m_triggerid);

//      std::cout << " ReadSensor for sensorid: " << sensorid << " with data.size() = " << data.size() << std::endl; 
//    }
    return result;
  }

  void  ExampleHardware::CompletedEvent() {
    m_triggerid++;
    m_nextevent += 1.0;
  }

} // namespace eudaq
