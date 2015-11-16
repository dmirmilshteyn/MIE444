using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Devices.Bluetooth.Rfcomm;
using Windows.Devices.Enumeration;
using Windows.Networking.Sockets;

namespace Tweak.Bluetooth
{
    class ConnectedDevice
    {
        public DeviceInformation Device { get; private set; }
        public RfcommDeviceService Service { get; private set; }
        public StreamSocket Socket { get; private set; }
        public Synchronizer Synchronizer { get; private set; }

        public ConnectedDevice(DeviceInformation device, RfcommDeviceService service, StreamSocket socket) {
            this.Device = device;
            this.Service = service;
            this.Socket = socket;

            this.Synchronizer = new Synchronizer(socket);
        }
    }
}
