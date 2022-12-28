using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace DiagonalUI.Logging
{
    class Line
    {
        public string Message { get; set; }
        public object Data { get; set; }
    }
    public delegate void LineHandler(object data);
    internal class LogParser
    {
        private readonly Dictionary<string, LineHandler> _handlers = new();

        private readonly string _tag;
        public LogParser(string tag)
        {
            _tag = tag;
        }

        public void AddHandler(string messageName, LineHandler handler)
        {
            _handlers.Add(messageName, handler);
        }

        public void ProcessLine(string line)
        {
            var index = line.IndexOf("/* log:tag:" + _tag + " */");
            if (index != -1)
            {
                var messageData = line.Substring(index, line.Length - 1);
                var deserialized = JsonSerializer.Deserialize<Line>(messageData);
                if(deserialized != null && _handlers.ContainsKey(deserialized.Message))
                {
                    _handlers.GetValueOrDefault(deserialized.Message)(deserialized.Data);
                }
            }
        }
    }
}
