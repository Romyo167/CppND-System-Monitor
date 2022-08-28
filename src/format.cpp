#include "format.h"

#include <string>

using std::string;
string formatSMH(int val){
  string str = "";
  if(val < 10 ) return ("0" +std::to_string(val));
  else return std::to_string(val);
return str;
}
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  unsigned int time_[3];
  for (int i = 0; i < 3; i++) {
    time_[i] = seconds % 60;
    seconds = (seconds - time_[i]) / 60;
  }
  string time_str;
  time_str = formatSMH(time_[2]) + ":" + formatSMH(time_[1]) + ":" +
             formatSMH(time_[0]);
  return time_str;
}
