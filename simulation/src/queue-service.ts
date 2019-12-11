import * as amqp from "amqplib/callback_api";
import { Message } from "amqplib/callback_api";

const QUEUE_NAME = "simulation";

export const startConsuming = (action: (message: Message) => Promise<void>) => {
  amqp.connect(process.env.QUEUE_URL as string, (_error, connection) => {
    connection.createChannel((_error, channel) => {
      channel.assertQueue(QUEUE_NAME, { durable: false });
      channel.prefetch(1);
      channel.consume(QUEUE_NAME, async message => {
        if (message) {
          await action(message);
          channel.ack(message);
        }
      });
    });
  });
};
