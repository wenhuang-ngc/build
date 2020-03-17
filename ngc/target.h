#pragma once
#include "pnt_data.h"
#include "observer.h"
#include "sensors.h"
#include <iostream> 
#include "rpc.h"
#include "rpc/client.h"

class Target : public Observer, public Subject
{
public:
  Distance  _d;
  Position _uav_pos;
  Track _track;
  int _frequency;
  int _cycle;

public:
  Target(int rate = 1) : _frequency(rate) {
    _cycle = static_cast<int> (((1.0 / _frequency) / (sleep_msec / 1000)));
  };
  ~Target() {};

  Track getTracking() { return _track; }
  
  void update(Subject* s) override;
  void updateRemote(Subject* s) override;
  void setUAV(Position const& p) {
      setUAVLocation(p);
  }
  void notify() override {
    for (auto e : _observers)
      e->update(this);
  }
int count = 0;
  void print_track() {
    std::cout << ++count << "\t\t--- Target TRACK ---" << std::endl
	      << "\t\t x=" << _track._pos._x << std::endl
	      << "\t\t y=" << _track._pos._y << std::endl
	      << "\t\t z=" << _track._pos._z << std::endl << std::endl;
  }

protected:
  void setDistance(Distance const& d)    { _d = d; }
  void setUAVLocation(Position const& p) { _uav_pos = p; }

private:
  void targetLocation();
};

