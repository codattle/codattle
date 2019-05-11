import { ObjectID } from 'bson';
import * as Dotenv from 'dotenv';
import * as Koa from 'koa';
import * as Router from 'koa-router';
import * as DAO from './dao';
import * as QueueService from './queue-service';
import { executeSimulation } from './simulation';

if (process.env.NODE_ENV !== 'production') {
  Dotenv.config();
}

const app = new Koa();
const router = new Router();

router.get('/', async (ctx) => {
  ctx.body = 'I am simulation service';
});

app.use(router.routes());
app.listen(3000);

DAO.connect(
  process.env.MONGODB_URI as string,
  process.env.MONGODB_USER as string,
  process.env.MONGODB_PASSWORD as string,
  process.env.MONGODB_DATABASE as string,
);

QueueService.startConsuming(async (message) => {
  const matchId = new ObjectID(message.content.toString('hex'));
  executeSimulation(matchId);
});
