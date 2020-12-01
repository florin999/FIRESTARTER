/******************************************************************************
 * FIRESTARTER - A Processor Stress Test Utility
 * Copyright (C) 2020 TU Dresden, Center for Information Services and High
 * Performance Computing
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/\>.
 *
 * Contact: daniel.hackenberg@tu-dresden.de
 *****************************************************************************/

#pragma once

#include <firestarter/Environment/Platform/PlatformConfig.hpp>

namespace firestarter::environment::platform {

class RuntimeConfig {
private:
  PlatformConfig *_platformConfig;
  payload::Payload *_payload;
  unsigned _thread;
  std::vector<std::pair<std::string, unsigned>> _payloadSettings;
  unsigned _instructionCacheSize;
  std::list<unsigned> _dataCacheBufferSize;
  unsigned _ramBufferSize;
  unsigned _lines;

public:
  RuntimeConfig(PlatformConfig *platformConfig, unsigned thread,
                unsigned detectedInstructionCacheSize)
      : _platformConfig(platformConfig), _payload(nullptr), _thread(thread),
        _payloadSettings(platformConfig->getDefaultPayloadSettings()),
        _instructionCacheSize(platformConfig->instructionCacheSize),
        _dataCacheBufferSize(platformConfig->dataCacheBufferSize),
        _ramBufferSize(platformConfig->ramBufferSize),
        _lines(platformConfig->lines) {
    if (detectedInstructionCacheSize != 0) {
      this->_instructionCacheSize = detectedInstructionCacheSize;
    }
  };
  RuntimeConfig(const RuntimeConfig &c)
      : _platformConfig(c.platformConfig),
        _payload(c.platformConfig->payload->clone()), _thread(c.thread),
        _payloadSettings(c.payloadSettings),
        _instructionCacheSize(c.instructionCacheSize),
        _dataCacheBufferSize(c.dataCacheBufferSize),
        _ramBufferSize(c.ramBufferSize), _lines(c.lines) {}
  ~RuntimeConfig();

  PlatformConfig *const &platformConfig = _platformConfig;
  payload::Payload *const &payload = _payload;
  const unsigned &thread = _thread;
  const std::vector<std::pair<std::string, unsigned>> &payloadSettings =
      _payloadSettings;
  const unsigned &instructionCacheSize = _instructionCacheSize;
  const std::list<unsigned> &dataCacheBufferSize = _dataCacheBufferSize;
  const unsigned &ramBufferSize = _ramBufferSize;
  const unsigned &lines = _lines;

  void setPayloadSettings(
      std::vector<std::pair<std::string, unsigned>> payloadSettings) {
    this->_payloadSettings = payloadSettings;
  }

  void setLineCount(unsigned lineCount) { this->_lines = lineCount; }

  void printCodePathSummary() {
    log::info() << "\n"
                << "  Taking " << platformConfig->payload->name
                << " path optimized for " << platformConfig->name << " - "
                << thread << " thread(s) per core\n"
                << "  Used buffersizes per thread:";

    if (instructionCacheSize != 0) {
      log::info() << "    - L1i-Cache: " << instructionCacheSize / thread
                  << " Bytes";
    }

    unsigned i = 1;
    for (auto const &bytes : dataCacheBufferSize) {
      log::info() << "    - L" << i << "d-Cache: " << bytes / thread
                  << " Bytes";
      i++;
    }

    log::info() << "    - Memory: " << ramBufferSize / thread << " Bytes";
  }
};

} // namespace firestarter::environment::platform
