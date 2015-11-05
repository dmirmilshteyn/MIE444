using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;
using Windows.Graphics.Imaging;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI;
using Windows.UI.Xaml.Media.Imaging;

namespace Tweak
{
    class MapLoader
    {
        public async Task<Map> LoadMapFromImage(StorageFile storageFile, Constants constants) {
            Map map = new Map();
            map.InitializeMap(constants);

            using (var stream = await storageFile.OpenAsync(FileAccessMode.Read)) {
                BitmapDecoder decoder = await BitmapDecoder.CreateAsync(stream);

                WriteableBitmap scaledImage = await ScaleImageToMap(map, decoder);

                byte[] pixels = scaledImage.PixelBuffer.ToArray();
                for (int i = 0; i < map.Tiles.Width * map.Tiles.Height; i++) {
                    byte b = pixels[i * 4];
                    byte g = pixels[(i * 4) + 1];
                    byte r = pixels[(i * 4) + 2];
                    byte a = pixels[(i * 4) + 3];

                    if (b == 0 && g == 0 && r == 0) {
                        map.Tiles.Tiles[i].Filled = true;
                    }
                }
            }

            return map;
        }

        private async Task<WriteableBitmap> ScaleImageToMap(Map map, BitmapDecoder decoder) {
            using (InMemoryRandomAccessStream stream = new InMemoryRandomAccessStream()) {
                BitmapEncoder encoder = await BitmapEncoder.CreateForTranscodingAsync(stream, decoder);

                encoder.BitmapTransform.ScaledWidth = (uint)map.Tiles.Width;
                encoder.BitmapTransform.ScaledHeight = (uint)map.Tiles.Height;

                await encoder.FlushAsync();

                WriteableBitmap scaledBitmap = new WriteableBitmap(map.Tiles.Width, map.Tiles.Height);
                await scaledBitmap.SetSourceAsync(stream);

                return scaledBitmap;
            }
        }
    }
}
