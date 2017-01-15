/*
 * This file is part of ATSDB.
 *
 * ATSDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ATSDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ATSDB.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Client.h
 *
 *  Created on: Jul 30, 2009
 *      Author: sk
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <QApplication>

//namespace ATSDB
//{

/**
 * @brief Main Class
 *
 */
class Client : public QApplication
{
public:
    ///@brief Constructor.
  Client(int& argc, char ** argv);
  ///@brief Destructor.
  virtual ~Client() { }

  ///@brief Re-implementation from QApplication so exceptions can be thrown in slots.
  virtual bool notify(QObject * receiver, QEvent * event);

};
//}

#endif /* CLIENT_H_ */