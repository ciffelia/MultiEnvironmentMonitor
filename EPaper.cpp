#include "EPaper.h"

#include <cstdio>
#include <cstring>

#include "TimeProvider.h"
#include "BatteryLowImage.h"
#include "TemperatureImage.h"
#include "HumidityImage.h"
#include "PressureImage.h"
#include "CO2Image.h"
#include "TVOCImage.h"
#include "DegreeImage.h"
#include "BootImage.h"
#include "SyncImage.h"
#include "Logger.h"

using namespace Constants::EPaper;

void EPaper::GetMeasuredValueStr(char *temperatureStr, char *humidityStr, char *pressureStr, char *co2Str, char *tvocStr, const Sensors::MeasuredValue value)
{
  sprintf(temperatureStr, "%.1f", value.temperature);
  sprintf(humidityStr, "%.1f%%", value.humidity);
  sprintf(pressureStr, "%.0fhPa", value.pressure);
  sprintf(co2Str, "%dppm", value.co2);
  sprintf(tvocStr, "%dppb", value.tvoc);
}

void EPaper::drawImage(const int xBase, const int yBase, const Image &image, const int colored)
{
  for (int y = 0; y < image.height; y++)
  {
    const int offset = y * image.width;
    for (int x = 0; x < image.width; x++)
    {
      if (image.data[offset + x])
      {
        paint.DrawPixel(xBase + x, yBase + y, colored);
      }
    }
  }
}

EPaper::EPaper()
  : paint(imageBuffer, Constants::EPaper::WIDTH, Constants::EPaper::HEIGHT)
{
}

void EPaper::begin()
{
  epd.Reset();
  const auto returnCode = epd.Init();
  if (returnCode != 0)
  {
    Throw("epd.Init() returned with an error: %d.", returnCode);
  }
}

void EPaper::sleep(){
  epd.Sleep();
}

void EPaper::drawStatusPage(const DateTime dateTime, const Sensors::MeasuredValue measuredValue, const bool isRTCBatteryLow)
{
  char dateStr[16], timeStr[16];
  TimeProvider::GetDateStr(dateStr, dateTime);
  TimeProvider::GetTimeStr(timeStr, dateTime, false);

  char temperatureStr[16], humidityStr[16], pressureStr[16], co2Str[16], tvocStr[16];
  GetMeasuredValueStr(temperatureStr, humidityStr, pressureStr, co2Str, tvocStr, measuredValue);
  
  const auto cellW = WIDTH;
  const auto cellH = HEIGHT / 6;
  const auto cellCX = cellW / 2;
  const auto cellCY = cellH / 2;

  const auto fontW = Font20.Width;
  const auto fontH = Font20.Height;

  const auto iconX = 20;
  const auto valueX = 70;
  
  paint.Clear(PAINT_WHITE);

  // 日時の背景の矩形
  paint.DrawFilledRectangle(0, 0, cellW, cellH - 1, PAINT_BLACK);

  // 各計測値を区切る直線
  paint.DrawHorizontalLine(0, cellH * 2, cellW, PAINT_BLACK);
  paint.DrawHorizontalLine(0, cellH * 3, cellW, PAINT_BLACK);
  paint.DrawHorizontalLine(0, cellH * 4, cellW, PAINT_BLACK);
  paint.DrawHorizontalLine(0, cellH * 5, cellW, PAINT_BLACK);

  // 各計測値のアイコン
  drawImage(iconX, cellH * 1 + cellCY - 16, TemperatureImage, PAINT_BLACK);
  drawImage(iconX, cellH * 2 + cellCY - 16, HumidityImage, PAINT_BLACK);
  drawImage(iconX, cellH * 3 + cellCY - 16, PressureImage, PAINT_BLACK);
  drawImage(iconX, cellH * 4 + cellCY - 16, CO2Image, PAINT_BLACK);
  drawImage(iconX, cellH * 5 + cellCY - 16, TVOCImage, PAINT_BLACK);

  // RTCの電池残量警告
  if (isRTCBatteryLow) {
    drawImage(WIDTH - fontW + 4, cellCY / 2 * 3 - fontH / 2 + 2 + 1, BatteryLowImage, PAINT_WHITE);
  }

  // 日時
  paint.DrawStringAt(cellCX - fontW * 10 / 2, cellCY / 2 - fontH / 2 + 3, dateStr, &Font20, PAINT_WHITE);
  paint.DrawStringAt(cellCX - fontW * 5 / 2, cellCY / 2 * 3 - fontH / 2 + 2, timeStr, &Font20, PAINT_WHITE);

  // 各計測値
  paint.DrawStringAt(valueX, cellH * 1 + cellCY - fontH / 2 + 2, temperatureStr, &Font20, PAINT_BLACK);
  paint.DrawStringAt(valueX, cellH * 2 + cellCY - fontH / 2 + 2, humidityStr, &Font20, PAINT_BLACK);
  paint.DrawStringAt(valueX, cellH * 3 + cellCY - fontH / 2 + 2, pressureStr, &Font20, PAINT_BLACK);
  paint.DrawStringAt(valueX, cellH * 4 + cellCY - fontH / 2 + 2, co2Str, &Font20, PAINT_BLACK);
  paint.DrawStringAt(valueX, cellH * 5 + cellCY - fontH / 2 + 2, tvocStr, &Font20, PAINT_BLACK);

  // 「℃」
  drawImage(
    valueX + strlen(temperatureStr) * fontW,
    cellH * 1 + cellCY - fontH / 2 + 1,
    DegreeImage,
    PAINT_BLACK
  );
  
  epd.DisplayFrame(imageBuffer);
}

void EPaper::drawBootPage()
{
  paint.Clear(PAINT_WHITE);
  
  drawImage(0, 0, BootImage, PAINT_BLACK);
  
  epd.DisplayFrame(imageBuffer);
}

void EPaper::drawSyncPage(){
  paint.Clear(PAINT_WHITE);

  drawImage(0, 0, SyncImage, PAINT_BLACK);

  epd.DisplayFrame(imageBuffer);
}
