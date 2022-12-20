#include <string>

#include "format.h"

using std::string;
using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hours=seconds/3600;
    long minutes=(seconds-(3600*hours))/60;
    long Seconds=(seconds-(3600*hours)-(60*minutes));
    string HH=to_string(hours);
    string MM=to_string(minutes);
    string SS=to_string(Seconds);
    MM.insert(0, 2-MM.length(), '0');
    SS.insert(0, 2-SS.length(), '0');

    return HH+":"+MM+":"+SS; 
}