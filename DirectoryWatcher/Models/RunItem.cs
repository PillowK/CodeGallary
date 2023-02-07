namespace DirectoryWatcher.Models
{
    public class RunItem
    {
        private string _runId;
        public RunItem(string filepath, string argument)
        {
            this._runId = Guid.NewGuid().ToString();
            this.Filepath = filepath;
            this.Argument = argument;
        }

        public string RunId { get { return _runId; } }
        public string Filepath { get; set; }
        public string Argument { get; set; }
    }
}
