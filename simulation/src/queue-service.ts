import * as amqp from 'amqplib/callback_api';
import { Message } from 'amqplib/callback_api';

const QUEUE_NAME = 'simulation';

export const startConsuming = (action: (message: Message) => void) => {
  amqp.connect(process.env.QUEUE_URL as string, (_error, connection) => {
    connection.createChannel((_error, channel) => {
      channel.assertQueue(QUEUE_NAME, { durable: false });
      channel.consume(QUEUE_NAME, (message) => {
        if (message) {
          action(message);
          channel.ack(message);
        }
      });
    });
  });
};
