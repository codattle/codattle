import { ObjectID } from 'bson';
import * as mongoose from 'mongoose';
import { List } from 'immutable';

export const findById = async <T extends mongoose.Document>(id: ObjectID, model: mongoose.Model<T>): Promise<T> => {
  const document = await model.findById(id);
  if (!document) {
    throw new Error(`Document with id ${id} doesn't exists.`);
  }
  return document;
};

export const findByIds = async <T extends mongoose.Document>(ids: ObjectID[], model: mongoose.Model<T>): Promise<List<T>> => {
  const documents = await model.find({ _id: { $in: ids } });
  return List(ids.map((id) => {
    const document = documents.find((docuement) => id.equals(docuement._id));
    if (!document) {
      throw new Error(`Document with id ${id} doesn't exists.`);
    }
    return document;
  }));
};

export const save = async <T extends mongoose.Document>(document: T): Promise<T> => {
  return await document.save();
};

export const connect = (uri: string, user: string, password: string, database: string) => {
  mongoose.connect(uri, { useNewUrlParser: true, user, pass: password, dbName: database }, (err) => {
    if (err) {
      console.error(err);
    }
  });
};
