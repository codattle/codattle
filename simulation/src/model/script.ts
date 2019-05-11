import { Document, model, Schema } from 'mongoose';
import { IGame } from './game';

export interface IScript extends Document {
  content: string;
  game: IGame['_id'];
}

const ScriptSchema: Schema = new Schema(
  {
    content: { type: String, required: true },
    game: { type: Schema.Types.ObjectId, required: true },
  },
  { collection: 'Script', versionKey: false },
);

export const Script = model<IScript>('Script', ScriptSchema);
