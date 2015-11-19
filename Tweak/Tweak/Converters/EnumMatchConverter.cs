using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Data;

namespace Tweak.Converters
{
    class EnumMatchConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language) {
            string valueString = ((Enum)value).ToString();

            return valueString.Equals(parameter);
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language) {
            bool bValue = (bool)value;
            if (bValue) {
                return Enum.Parse(typeof(MapPlacementMode), (string)parameter);
            } else {
                return null;
            }
        }
    }
}
