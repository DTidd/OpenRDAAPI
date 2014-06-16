// This may look like C code, but it's really -*- C++ -*-
#ifndef NUMERIC_FIELD_H_
#define NUMERIC_FIELD_H_

#include <Wt/WLineEdit>
#include <Wt/WString>

class NumericConversionError : public std::exception {
public:
  NumericConversionError(const std::string& description)
    : what_(description)
  { }

  ~NumericConversionError() throw()
  { }

  const char *what() throw() {
    return what_.c_str();
  }
private:
  std::string what_;
};

class NumericField : public Wt::WLineEdit
{
public:
  /* Create a new NumericField with the given parent */
  NumericField(Wt::WContainerWidget *parent = 0);

  /* Configures whether decimals are allowed, ignored if this
   * is a currency field */
  void setDecimal(bool decimal);
  bool decimal() const { return decimal_; }

  /* Configures total amount of digits allowed */
  void setDigits(int digits);
  int intDigits() const { return digits_; }

  /* Configures whether negative numbers are allowed */
  void setAllowNegative(bool allowNegative);
  bool allowNegative() const { return allowNegative_; }

  /* Set this input as a currency input, with the given currency symbol,
   * the given amount of digits after the dot (usually 0 or 2), and
   * the characters to pad with.
   *
   * currencySymbol and padChar are supposed to be a single character,
   * otherwise behavior is undefined. */
  void setCurrencyInput(const Wt::WString& currencySymbol, int cents = 0,
			const Wt::WString& padChar = " ");

  /* Get the currency symbol. */
  const Wt::WString& currencySymbol() const  { return currencySymbol_; }

  /* Get the amount of digits after the dot, for a currency. */
  int currencyCentsDigits() const  { return currencyCents_; }

  /* Get the currency padding character. */
  const Wt::WString& padChar() const { return padChar_; }

  /* Get whether this is a currency input. */
  bool currency() const { return currency_; }

  /* Set the value of this input to the given unsigned integer.
   *
   * If this is a currency input, the value should be in cents. */
  void setValue(unsigned value);

  /* Set the value of this input to the given signed integer.
   *
   * If this is a currency input, the value should be in cents.
   *
   * Throws a NumericConversionError if a negative value is supplied and this NumericField does
   * not allow negative values. */
  void setValue(int value);

  /* Set the value of this input to the given double.
   *
   * If this is a currency input, the value should be in cents.
   *
   * Throws a NumericConversionError if a negative value is supplied and this NumericField does
   * not allow negative values. */
  void setValue(double value);

  /* Get the value of this input as an unsigned integer.
   *
   * If this is a currency input, the value returned is in cents.
   *
   * Throws a NumericConversionError if the value is negative, or
   * if the value was not in the right format. */
  unsigned unsignedValue() const;

  /* Get the value of this input as a signed integer.
   *
   * If this is a currency input, the value returned is in cents.
   *
   * Throws a NumericConversionError if the value was not
   * in the right format. */
  int intValue() const;

  /* Get the value of this input as a double.
   *
   * If this is a currency input, the value returned is in cents.
   *
   * Throws a NumericConversionError if the value was not
   * in the right format. */
  double doubleValue() const;

  void render(Wt::WFlags<Wt::RenderFlag> flags);

private:
  int calculateMaxLength() const;
  Wt::WString defaultText() const;
  Wt::WString formatDouble(double d) const;

  int digits_;
  bool decimal_, allowNegative_, currency_;
  Wt::WString currencySymbol_, padChar_;
  int currencyCents_;
};

#endif // NUMERIC_FIELD_H_
