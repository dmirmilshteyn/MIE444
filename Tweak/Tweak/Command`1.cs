using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Tweak
{
    class Command<T> : ICommand
    {
        public event EventHandler CanExecuteChanged;

        Action<T> execute;

        public Command(Action<T> execute) {
            this.execute = execute;
        }

        public bool CanExecute(object parameter) {
            return true;
        }

        public void Execute(object parameter) {
            execute((T)parameter);
        }
    }
}
