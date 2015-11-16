using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.Devices.Bluetooth.Rfcomm;
using Windows.Devices.Enumeration;
using Windows.Networking.Sockets;

namespace Tweak.Bluetooth
{
    class BluetoothManager
    {
        public ObservableCollection<DeviceInformation> AvailableDevices { get; private set; }

        public BluetoothManager() {
            AvailableDevices = new ObservableCollection<DeviceInformation>();
        }

        public async Task Search() {
            var services = await DeviceInformation.FindAllAsync(RfcommDeviceService.GetDeviceSelector(RfcommServiceId.SerialPort));
            foreach (var device in services) {
                AvailableDevices.Add(device);
            }
        }

        public async Task<ConnectedDevice> Connect(DeviceInformation device) {
            var service = await RfcommDeviceService.FromIdAsync(device.Id);

            var socket = new StreamSocket();
            await socket.ConnectAsync(service.ConnectionHostName, service.ConnectionServiceName, SocketProtectionLevel.BluetoothEncryptionAllowNullAuthentication);

            ConnectedDevice connectedDevice = new ConnectedDevice(device, service, socket);
            await connectedDevice.Synchronizer.Synchronize();
            return connectedDevice;
        }
    }
}
