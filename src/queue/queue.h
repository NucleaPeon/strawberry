/*
 * Strawberry Music Player
 * This file was part of Clementine.
 * Copyright 2010, David Sansome <me@davidsansome.com>
 *
 * Strawberry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Strawberry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Strawberry.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "config.h"

#include <QObject>
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QList>
#include <QVariant>
#include <QString>
#include <QStringList>

class QMimeData;
class Playlist;

class Queue : public QAbstractProxyModel {
  Q_OBJECT

 public:
  Queue(Playlist *parent = nullptr);

  static const char *kRowsMimetype;

  // Query the queue
  bool is_empty() const;
  int PositionOf(const QModelIndex &source_index) const;
  bool ContainsSourceRow(int source_row) const;
  int PeekNext() const;
  int ItemCount() const;
  quint64 GetTotalLength() const;

  // Modify the queue
  int TakeNext();
  void ToggleTracks(const QModelIndexList &source_indexes);
  void InsertFirst(const QModelIndexList &source_indexes);
  void Clear();
  void Move(const QList<int> &proxy_rows, int pos);
  void MoveUp(int row);
  void MoveDown(int row);
  void Remove(QList<int> &proxy_rows);

  // QAbstractProxyModel
  void setSourceModel(QAbstractItemModel *source_model);
  QModelIndex mapFromSource(const QModelIndex &source_index) const;
  QModelIndex mapToSource(const QModelIndex &proxy_index) const;

  // QAbstractItemModel
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &proxy_index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QStringList mimeTypes() const;
  Qt::DropActions supportedDropActions() const;
  QMimeData *mimeData(const QModelIndexList &indexes) const;
  bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
  Qt::ItemFlags flags(const QModelIndex &index) const;

 public slots:
  void UpdateSummaryText();

 signals:
  void TotalLengthChanged(const quint64 length);
  void ItemCountChanged(const int count);
  void SummaryTextChanged(const QString& message);

 private slots:
  void SourceDataChanged(const QModelIndex &top_left, const QModelIndex &bottom_right);
  void SourceLayoutChanged();
  void UpdateTotalLength();

 private:
  QList<QPersistentModelIndex> source_indexes_;
  const Playlist *playlist_;
  quint64 total_length_ns_;

};

#endif  // QUEUE_H
