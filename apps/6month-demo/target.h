#pragma once
#include "pnt_data.h"
#include "observer.h"
#include "sensors.h"
#include <iostream> 
#include "rpc.h"

class Target : public Observer, public Subject
{
public:
  Distance  _d;
  Position _uav_pos;
  Track _track;
  int _frequency;
  int _cycle;
  int _count = 0;

public:
  Target(int rate = 1) : _frequency(rate) {
    _cycle = static_cast<int> (((1.0 / _frequency) / (sleep_msec / 1000)));
  };
  ~Target() {};

  Track getTracking() { return _track; }
  
  void update(Subject* s) override;

  void notify() override {
    for (auto e : _observers)
      e->update(this);
  }

  void print_track() {
    std::cout << ++_count << "\t\t--- Target TRACK ---" << std::endl
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

class TargetShadow: public Target, public Trailer
{
public:
  TargetShadow(int rate = 1) {
  };
  ~TargetShadow() {};

  void notify() override {
      Target::notify();
  }

  virtual void update(Subject *s) override;

};
