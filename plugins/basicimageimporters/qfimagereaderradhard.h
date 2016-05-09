/*
Copyright (c) 2008-2015 Jan Buchholz & Jan W. Krieger, German Cancer Research Center

    

    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QFIMAGEREADERRADHARD_H
#define QFIMAGEREADERRADHARD_H

#include "qfimporterimageseries.h"
#include "frame.h"
#include "crc_ccitt.h"
#include <QFile>
#include <QDataStream>
#include <stdint.h>

#define MAX_FRAME_SIZE (64*1024)

/*! \brief image reader class for Radhard raw images (implements QFRDRImageReader)
    \ingroup qf3rdrdp_imaging_fcs

    \note <<<THIS CODE IS NOT PLATFORM INDEPENDENT>>>
    This class is used to read an image series frame by frame. It is used in this manner:
*/
class QFImageReaderRadhard : public QFImporterImageSeries
{
    public:
        QFImageReaderRadhard();
        ~QFImageReaderRadhard();
        /** \brief open the given image sequence file
         *  \param filename name of the image sequence file
         *  \return \c true on success
         */
        virtual bool open(QString filename);
        /** \brief close the currently opened image file */
        virtual void close();
        /** \brief move the reading pointer back to the first frame */
        virtual void reset();
        /** \brief return the number of frames in the files.
         *
         *  This does not change the state of the class. This method may have a long runtime, if it has to really count the frames!
         */
        virtual uint32_t countFrames();
        /** \brief move on to the next frame in the file. return \c false if no further image exists */
        virtual bool nextFrame();
        /** \brief return a filter string for the file format */
        virtual QString filter() const;
        /** \brief return a name string for the file format */
        virtual QString formatName() const ;

    protected:
        /** \brief return the width of the frames (valid after open() returned \c true */
        virtual uint32_t intFrameWidth();
        /** \brief return the height of the frames (valid after open() returned \c true */
        virtual uint32_t intFrameHeight();
        virtual uint32_t intRawDataFormat() {return QF_RDF_PACKED1B;}
        /** \brief read a new frame into the given array of floating point numbers */
        virtual bool intReadFrameFloat(float* data, int channel=0);
        /** \brief read a new frame into the given array of integers */
        virtual bool intReadFrameUINT16(uint16_t* data, int channel=0);
        virtual bool intReadFrameDouble(double* data, int channel=0);


        uint32_t width;
        uint32_t height;
        QString filename;
        unsigned int frameSize;
        QFile *file;
        unsigned char* buffer;

        unsigned int calculateFrameSize();

        template < typename T >
        inline bool readFrame_(T* data) {
          if (!file) return false;
          bool result=false;
          frame<T> *f = new frame<T>(data,width,height);
          if(buffer[0]==0xFFU) {
            f->template load_packed<uint32_t>((uint32_t*)&(buffer[4]));
            result=true;
          } else {
            setLastError(QObject::tr("did not find 0xFF at start of frame, found %1").arg(buffer[0],0,16));
          }
          delete f;
          return result;
        }
};

#endif // QFIMAGEREADERRADHARD_H
