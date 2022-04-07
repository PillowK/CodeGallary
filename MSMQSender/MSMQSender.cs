using Microsoft.SqlServer.Server;
using System.Messaging;

namespace MSMQ
{

    public class MSMQSender
    {
        [SqlProcedure]
        public static void SendMessage(string queuePath, string messageLable, string messageBody)
        {
            if(MessageQueue.Exists(queuePath))
            {
                MessageQueue mq = new MessageQueue(queuePath);
                Message mm = new Message();
                mm.Label = messageLable;
                mm.Body = messageBody;
                mq.Send(mm);

                mq.Close();
            }
        }
    }
}



