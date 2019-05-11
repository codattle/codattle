import { Document, model, Schema } from 'mongoose';
import { IGame } from './game';
import { IResult, ResultSchema } from './result';
import { IScript } from './script';

export interface IMatch extends Document {
  name: string;
  game: IGame['_id'];
  scripts: Array<IScript['id']>;
  result: IResult;
}

const MatchSchema: Schema = new Schema(
  {
    name: { type: String, required: true },
    game: { type: Schema.Types.ObjectId, required: true },
    scripts: { type: [Schema.Types.ObjectId], required: true },
    result: { type: ResultSchema },
  },
  { collection: 'Match', versionKey: false },
);

export const Match = model<IMatch>('Match', MatchSchema);
