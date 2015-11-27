using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Bluetooth
{
    enum BTCommand
    {
        RequestSync = '!',
        SyncMap = 'M',
        OutputState = '0',
        SetP = '1',
        SetI = '2',
        SetD = '3',
        SpeedAdjust = 'z',
        AverageSpeed = 'x',
        StallPWM = 'y'
    }
}
