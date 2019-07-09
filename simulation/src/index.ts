import * as Dotenv from 'dotenv';
import * as Koa from 'koa';
import * as Router from 'koa-router';
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

QueueService.startConsuming(async (message) => {
  const matchId = message.content.readInt32BE(0);
  try {
    executeSimulation(matchId.toString());
  } catch(err) {
    console.error(err);
  }
});
