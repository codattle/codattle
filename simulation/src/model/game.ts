import { Document, model, Schema } from 'mongoose';

export interface IGame extends Document {
  name: string;
  mainLoop: string;
}

const GameSchema: Schema = new Schema(
  {
    name: { type: String, required: true },
    mainLoop: { type: String, required: true },
  },
  { collection: 'Game', versionKey: false },
);

export const Game = model<IGame>('Game', GameSchema);
