/*******************************************************************************
 * Copyright (c) 2013, Art Clarke.  All rights reserved.
 *  
 * This file is part of Humble-Video.
 *
 * Humble-Video is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Humble-Video is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Humble-Video.  If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************/
/*
 * Encoder.h
 *
 *  Created on: Jul 23, 2013
 *      Author: aclarke
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <io/humble/video/Coder.h>
#include <io/humble/video/MediaPacket.h>
#include <io/humble/video/MediaAudio.h>
#include <io/humble/video/MediaPicture.h>
#include <io/humble/video/MediaSubtitle.h>
#include <io/humble/video/MediaAudioResampler.h>

namespace io {
namespace humble {
namespace video {

/**
 * Encodes MediaAudio, MediaPicture or MediaSubtitle objects into MediaPacket objects.
 */
class VS_API_HUMBLEVIDEO Encoder : public io::humble::video::Coder
{
public:
  /**
   * Create a Encoder that will use the given Codec.
   *
   * @return a Encoder
   * @throws InvalidArgument if codec is null or codec cannot decode.
   */
  static Encoder* make(Codec* codec);

  /**
   * Creates a Encoder from a given Encoder
   * @return a Encoder
   * @throws InvalidArgument if src is null
   */
  static Encoder* make(Coder* src);

#ifndef SWIG
  static Encoder* make(Codec* codec, AVCodecContext* src);
#endif // ! SWIG

  /**
   * Open this Coder, using the given bag of Codec-specific options.
   *
   * @param inputOptions If non-null, a bag of codec-specific options.
   * @param unsetOptions If non-null, the bag will be emptied and then filled with
   *                     the options in <code>inputOptions</code> that were not set.
   *
   */
  virtual void open(KeyValueBag* inputOptions, KeyValueBag* unsetOptions);

  /**
   * Encode the given MediaPicture using this encoder.
   *
   * The MediaPicture will allocate a buffer to use internally for this, and
   * will free it when the frame destroys itself.
   *
   * Also, when done in order to flush the encoder, caller should call
   * this method passing in 0 (null) for frame to tell the encoder
   * to flush any data it was keeping a hold of.
   *
   * @param output [out] The packet to encode into.  It will point
   *     to a buffer allocated in the frame.  Caller should check MediaPacket.isComplete()
   *     after call to find out if we had enough information to encode a full packet.
   * @param frame [in/out] The frame to encode
   *
   */
  virtual void encodeVideo(MediaPacket * output,
      MediaPicture * frame);

  /**
   * Encode the given MediaAudio using this encoder.
   *
   * Callers should call this repeatedly on a set of samples until
   * we consume all the samples.
   *
   * Also, when done in order to flush the encoder, caller should call
   * this method passing in 0 (null) for samples to tell the encoder
   * to flush any data it was keeping a hold of.
   *
   * @param output [out] The packet to encode into.  It will point
   *          to a buffer allocated in the frame.  Caller should check MediaPacket.isComplete()
   *     after call to find out if we had enough information to encode a full packet.
   * @param samples [in] The samples to consume
   *
   */
  virtual void encodeAudio(MediaPacket * output,
      MediaAudio* samples);

protected:
  Encoder(Codec*, AVCodecContext* src, bool copySrc);
  virtual
  ~Encoder();
private:
  // Used to ensure we have the right frame-size for codecs that
  // require fixed frame sizes on audio.
  io::humble::ferry::RefPointer<MediaAudioResampler> mAResampler;
  io::humble::ferry::RefPointer<MediaAudio> mResampledAudio;

  int64_t mLastPtsEncoded;
};

} /* namespace video */
} /* namespace humble */
} /* namespace io */
#endif /* ENCODER_H_ */
