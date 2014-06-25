#include "NumericField.h"
#include<guip.hpp>
#include<misc.hpp>

#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>

#include <Wt/WApplication>
#include <Wt/WLineEdit>

NumericField::NumericField(Wt::WContainerWidget *parent)
  : Wt::WLineEdit(parent),
    digits_(8),
    decimal_(false),
    allowNegative_(false),
    currency_(false),
    currencySymbol_(Wt::WString::fromUTF8("$")),
    padChar_(Wt::WString::fromUTF8(" ")),
    currencyCents_(2)
{
  addStyleClass("numeric_field");
  Wt::WApplication::instance()->require("NumericField.js");
  keyWentDown().connect("jQuery.data(" + jsRef() + ", 'NumericField').keyWentDown");
  keyPressed().connect("jQuery.data(" + jsRef() + ", 'NumericField').keyPressed");
}

void NumericField::render(Wt::WFlags<Wt::RenderFlag> flags)
{
  setTextSize(calculateMaxLength());
  setMaxLength(calculateMaxLength());
  if (text().empty()) {
    setText(defaultText());
  }

  Wt::WLineEdit::render(flags);

  std::stringstream config;
  config << "{"
    "allowNegative:" << (allowNegative_ ? "true" : "false") <<
    ",decimal:" << (decimal_ ? "true" : "false") <<
    ",digits:" << digits_ <<
    ",currency:" << (currency_ ? "true" : "false") <<
    ",currencyCents:" << currencyCents_ <<
    ",padChar:" << jsStringLiteral(padChar_.toUTF8()) <<
    ",currencySymbol:" << jsStringLiteral(currencySymbol_.toUTF8()) <<
    "}";
  setJavaScriptMember("NumericField",
      "new NumericField(" + jsRef() + "," + config.str() + ")");
}

int NumericField::calculateMaxLength() const
{
  int textSize = digits_;
  if (decimal_ || (currency_ && currencyCents_ > 0)) {
    ++ textSize;
  }
  if (currency_) {
    ++ textSize;
    int nbDigitsBeforeDot = digits_ - currencyCents_;
    int nbCommas = nbDigitsBeforeDot / 3;
    if (nbDigitsBeforeDot % 3 == 0) {
      -- nbCommas;
    }
    textSize += nbCommas;
  }
  if (allowNegative_) {
    ++ textSize;
  }
  return textSize;
}

Wt::WString NumericField::defaultText() const
{
  return formatDouble(0.0);
}

Wt::WString NumericField::formatDouble(double d) const
{
  bool sign = false;
  if (d < 0) {
    sign = true;
    d = -d;
  }
  double decimals = 0.0;
  if (currency_) {
    double factor = pow(10.0, currencyCents_ == 0 ? 2 : currencyCents_);
    decimals = floor(d - (floor(d / factor) * factor));
    d = floor(d / factor);
  }
  if (d != d || d == std::numeric_limits<double>::infinity()) {
    throw NumericConversionError("The given double is not finite (NaN or infinity)!");
  }
  int intPartLength = d < 1.0 ? 1 : (int) (log10(d) + 1);
  if (intPartLength > digits_) {
    throw NumericConversionError("The given double value is larger than the number of allowed digits!");
  }
  Wt::WLocale locale;
  if (currency_) {
    locale.setGroupSeparator(",");
  }
  int precision = 0;
  if (!currency_ && decimal_) {
/* added code below */
    std::stringstream ss1;
    char *temp=NULL,*tmp=NULL;
    short count=0,found_period=FALSE,x=0;
    ss1 << d; 
    tmp=ss1.str().c_str();
    if(!isEMPTY(tmp))
    {
     for(x=0,temp=tmp;*temp;++x,++temp) 
     {
      if(*temp=='.') found_period=x;
      if(x>found_period) ++count;
     }
     if(count<(digits_ -intPartLength)) precision=count;
	else precision=digits_ - intPartLength;
   } else {
/* added code above */
     precision = digits_ - intPartLength;
   }
  }
  Wt::WString result = locale.toFixedString(d, precision);
  std::size_t size = result.toUTF8().size();
  if (!currency_ && precision > 0) {
    // Remove trailing zeros after dot
    while (result.toUTF8()[size - 1] == '0') {
      -- size;
    }
    if (result.toUTF8()[size - 1] == '.') {
      -- size;
    }
    result = Wt::WString::fromUTF8(std::string(result.toUTF8().c_str(), size));
  }
  if (currency_ && currencyCents_ > 0) {
    result = result + ".";
    for (std::size_t i = boost::lexical_cast<std::string>(decimals).size(); i < currencyCents_; ++i) {
      result = result + "0";
    }
    result = result + boost::lexical_cast<std::string>(decimals);
    size += currencyCents_ + 1;
  }
  if (allowNegative_) {
    if (sign) {
      result = result + "-";
    } else {
      result = result + " ";
    }
    ++ size;
  } else if (sign) {
    throw NumericConversionError("Can't assign a negative value to an unsigned NumericField!");
  }
  if (currency_) {
    // Add padding and currency symbol
    int length = calculateMaxLength();
    size_t curLength = size;
    for (size_t i = 1; i < length - curLength; ++i) {
      result = padChar_ + result;
    }
    result = currencySymbol_ + result;
  }
  return result;
}

void NumericField::setDecimal(bool decimal)
{
  decimal_ = decimal;
}

void NumericField::setDigits(int digits)
{
  digits_ = digits;
}

void NumericField::setAllowNegative(bool allowNegative)
{
  allowNegative_ = allowNegative;
}

void NumericField::setCurrencyInput(const Wt::WString& currencySymbol, int cents,
				    const Wt::WString& padChar)
{
  currency_ = true;
  currencySymbol_ = currencySymbol;
  currencyCents_ = cents;
  padChar_ = padChar;
}

void NumericField::setValue(unsigned value)
{
  setText(formatDouble((double) value));
}

void NumericField::setValue(int value)
{
  setText(formatDouble((double) value));
}

void NumericField::setValue(double value)
{
  setText(formatDouble(value));
}

unsigned NumericField::unsignedValue() const
{
  double result = doubleValue();
  if (result < 0) {
    throw NumericConversionError("The value of the NumericField was negative, "
	"but an unsigned value was requested!");
  }
  return (unsigned) result;
}

int NumericField::intValue() const
{
  return (int) doubleValue();
}

double NumericField::doubleValue() const
{
  std::string value = text().toUTF8();
  bool sign = false;
  if (allowNegative_) {
// Extract and remove sign
    if (!value.empty()) {
      if (value[value.size() - 1] == '-') {
	sign = true;
      }
      if (sign || value[value.size() - 1] == ' ') {
	value = value.substr(0, value.size() - 1);
      }
    }
  }
  std::size_t extraZeros = 0;
  if (currency_) {
    // Remove currency symbol and padding
    std::size_t symbolSize = currencySymbol_.toUTF8().size();
    std::size_t start = 0;
    if (value.substr(0, symbolSize) == currencySymbol_.toUTF8()) {
      start += symbolSize;
    }
    std::size_t padCharSize = padChar_.toUTF8().size();
    while (value.substr(start, padCharSize) == padChar_.toUTF8()) {
      start += padCharSize;
    }
    value = value.substr(start);
    // Remove commas
    std::string::iterator end = std::remove(value.begin(), value.end(), ',');
    std::string::iterator dotPos = std::find(value.begin(), end, '.');
    if (dotPos + currencyCents_ + 1 < end) {
      // Truncate extraneous digits
      end = std::remove(value.begin(), dotPos + currencyCents_ + 1, '.');
    } else {
      end = std::remove(value.begin(), end, '.');
      extraZeros = dotPos + currencyCents_ - end;
    }
    value = std::string(value.begin(), end);
  }
  double result = 0.0;
  try {
    result = boost::lexical_cast<double>(value);
    for (std::size_t i = 0; i < extraZeros; ++i) {
      result *= 10.0;
    }
    if (sign) {
      result *= -1.0;
    }
    if (currency_ && currencyCents_ == 0) {
      result *= 100.0;
    }
  } catch (boost::bad_lexical_cast&) {
    throw NumericConversionError("Couldn't convert text: " + value);
  } 
  return result;
}

