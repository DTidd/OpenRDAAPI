function NumericField(edit, config) {
  var BACKSPACE_KEYCODE = 8;
  var DELETE_KEYCODE = 46;
  var RIGHT_KEYCODE = 39;
  var LEFT_KEYCODE = 37;
  var HOME_KEYCODE = 36;
  var END_KEYCODE = 35;

  jQuery.data(edit, 'NumericField', this);

  function isDecimal() {
    return config.decimal && !config.currency;
  }

  function getSelection() {
    if (document.selection) { // IE
      var start = -1;
      var end = -1;

      var val = $(edit).val();
      if (val) {
	var range = document.selection.createRange().duplicate();

	range.moveEnd("character", val.length);
	start = (range.text == "" ? val.length : val.lastIndexOf(range.text));

	range = document.selection.createRange().duplicate();
	range.moveStart("character", -val.length);
	end = range.text.length;
      }

      return {start: start, end: end}
    } else if (edit.selectionStart || edit.selectionStart == 0) {
      return {start: edit.selectionStart, end: edit.selectionEnd};
    } else
      return {start: -1, end: -1};
  }

  function setSelection(start, end) {
    /**
     * @preserve Include jQuery Caret Range plugin
     * Copyright (c) 2009 Matt Zabriskie
     * Released under the MIT and GPL licenses.
     */
    var val = $(edit).val();

    if (typeof start !== 'number') start = -1;
    if (typeof end !== 'number') end = -1;
    if (start < 0) start = 0;
    if (end > val.length) end = val.length;
    if (end < start) end = start;
    if (start > end) start = end;

    if (config.allowNegative) {
      if (start > val.length - 1) start = val.length - 1;
      if (end > val.length - 1) end = val.length - 1;
    }

    edit.focus();

    if (typeof edit.selectionStart !== 'undefined') {
      edit.selectionStart = start;
      edit.selectionEnd = end;
    } else if (document.selection) {
      var range = edit.createTextRange();
      range.collapse(true);
      range.moveStart('character', start);
      range.moveEnd('character', end - start);
      range.select();
    }
  }

  // Remove the leading zeros of the given value.
  // Only numeric characters and dots are supported.
  // Padding characters are assumed not to be in the given value.
  function removeLeadingZeros(value) {
    var i = 0;
    var lastLeadingZeroPos = -1;
    for (i = 0; i < value.length; i++) {
      if (value.charAt(i) === '0') {
	lastLeadingZeroPos = i;
      } else {
	break;
      }
    }
    var result = value.substring(lastLeadingZeroPos + 1);
    // Add zero before . back in
    if (result === "" || result.charAt(0) === '.') {
      result = '0' + result;
    }
    return result;
  }

  // Clear the range from character start to before end.
  function clearRange(value, start, end) {
    return value.substring(0, start) + value.substring(end);
  }

  // Return whether the field is negative. Only to be used if allowNegative is enabled.
  function getSign(value) {
    return value.charAt(value.length - 1) === '-';
  }

  // Return the value without the sign. Only to be used if allowNegative is enabled.
  function clearSign(value) {
    return value.substring(0, value.length - 1);
  }

  // Insert the sign again. Only to be used if allowNegative is enabled.
  function insertSign(value, sign) {
    return value + (sign ? '-' : ' ');
  }

  // Remove the dot, for currencies.
  function removeDot(value, selection) {
    if (config.currencyCents === 0) {
      return value;
    }
    var dotPos = value.indexOf('.');
    if (dotPos !== -1) {
      if (selection.start > dotPos) {
	selection.start = selection.start - 1;
      }
      if (selection.end > dotPos) {
	selection.end = selection.end - 1;
      }
      return value.substring(0, dotPos) + value.substring(dotPos + 1);
    }
    return value;
  }

  // Add the dot again, for currencies.
  function addDot(value, selection) {
    if (config.currencyCents === 0) {
      return value;
    }
    var dotPos = value.length - config.currencyCents;
    if (selection.start > dotPos) {
      selection.start = selection.start + 1;
    }
    if (selection.end > dotPos) {
      selection.end = selection.end + 1;
    }
    var end = value.substring(dotPos);
    for (var i = end.length; i < config.currencyCents; i++) {
      end = '0' + end;
    }
    return value.substring(0, dotPos) + '.' + end;
  }

  // Remove all commas, correcting the selection accordingly, for currencies.
  function removeCommas(value, selection) {
    var result = "";
    var commaPos = value.indexOf(',');
    var prevCommaPos = -1;
    var nbCommas = 0;
    while (commaPos !== -1) {
      result += value.substring(prevCommaPos + 1, commaPos);
      if (selection.start > prevCommaPos && selection.start <= commaPos) {
	selection.start = selection.start - nbCommas;
      }
      if (selection.end > prevCommaPos && selection.end <= commaPos) {
	selection.end = selection.end - nbCommas;
      }
      prevCommaPos = commaPos;
      commaPos = value.indexOf(',', commaPos + 1);
      nbCommas ++;
    }
    result += value.substring(prevCommaPos + 1);
    if (selection.start > prevCommaPos) {
      selection.start = selection.start - nbCommas;
    }
    if (selection.end > prevCommaPos) {
      selection.end = selection.end - nbCommas;
    }
    return result;
  }

  // Add in commas, correcing the selection accordingly, for currencies.
  function addCommas(value, selection) {
    var signChar = null;
    if (value.charAt(value.length - 1) === ' ' || value.charAt(value.length - 1) === '-') {
      signChar = value.charAt(value.length - 1);
      value = value.substring(0, value.length - 1);
    }
    var startFromEnd = value.length - selection.start;
    var endFromEnd = value.length - selection.end;
    var dotPos = value.indexOf('.');
    var result = "";
    var pos = value.length - 1;
    if (dotPos !== -1) {
      result = value.substring(dotPos);
      pos = dotPos - 1;
    }
    var i = 0;
    var prevCommaPos = -1;
    for (; pos > -1; pos--) {
      if (i % 3 === 0 && i !== 0) {
	result = "," + result;
	if (selection.start <= prevCommaPos) {
	  startFromEnd ++;
	}
	if (selection.end <= prevCommaPos) {
	  endFromEnd ++;
	}
	prevCommaPos = pos;
      }
      result = value.charAt(pos) + result;
      i++;
    }
    if (selection.start <= prevCommaPos) {
      startFromEnd ++;
    }
    if (selection.end <= prevCommaPos) {
      endFromEnd ++;
    }
    selection.start = result.length - startFromEnd;
    selection.end = result.length - endFromEnd;
    if (signChar) {
      result = result + signChar;
    }
    return result;
  }

  // Remove currency sign and padding, for currency input.
  function removeCurrencySignAndPadding(value, selection) {
    var newValue = value;
    if (newValue.charAt(0) === config.currencySymbol) {
      newValue = newValue.substring(1); // Remove sign
    }
    selection.start --;
    selection.end --;
    var lastPadChar = -1;
    for (var i = 0; i < newValue.length; i++) {
      if (newValue.charAt(i) === config.padChar) {
	lastPadChar = i;
      } else {
	break;
      }
    }
    newValue = newValue.substring(lastPadChar + 1);
    selection.start -= (lastPadChar + 1);
    selection.end -= (lastPadChar + 1);
    if (selection.start < 0) selection.start = 0;
    if (selection.end < 0) selection.end = 0;
    return newValue;
  }

  // Add currency sign and padding, for currency input.
  function addSignAndPadding(value, selection) {
    var nbDigitsBeforeDot = config.digits - config.currencyCents;
    var nbCommas = Math.floor(nbDigitsBeforeDot / 3);
    if (nbDigitsBeforeDot % 3 === 0) {
      nbCommas --;
    }
    var maxLength = config.digits + nbCommas;
    if (config.currencyCents > 0) {
      maxLength ++;
    }
    if (config.allowNegative) {
      maxLength ++;
    }
    var paddingLength = maxLength - value.length;
    var newValue = value;
    for (var i = 0; i < paddingLength; i++) {
      newValue = config.padChar + newValue;
    }
    newValue = config.currencySymbol + newValue;
    selection.start += paddingLength + 1;
    selection.end += paddingLength + 1;
    return newValue;
  }

  // Remove all of the extra stuff: sign, padding, commas,...
  // The selection is adjusted accordingly, or enough state is preserved
  // to restore it correctly afterwards.
  function removeExtraStuff(state) {
    if (config.currency) {
      state.value = removeCurrencySignAndPadding(state.value, state.selection);
      state.value = removeCommas(state.value, state.selection);
    }
    state.posFromEnd = state.value.length - state.selection.end;
    state.sign = false;
    var dotPos = -1;
    if (config.allowNegative) {
      state.sign = getSign(state.value);
      state.value = clearSign(state.value);
    }
    if (config.currency) {
      state.value = removeDot(state.value, state.selection);
    }
  }

  // Add the extra stuff back in: sign, padding, commas...
  // The selection is restored correctly. If moveRight is true,
  // then the cursor moves right, e.g. when doing a delete.
  function addExtraStuff(state, moveRight) {
    if (config.currency) {
      state.value = addDot(state.value, state.selection);
    }
    state.value = removeLeadingZeros(state.value, state.selection);
    if (config.allowNegative) {
      state.value = insertSign(state.value, state.sign);
    }
    if (moveRight) {
      var newPos = state.value.length - state.posFromEnd + 1;
      if (state.value.charAt(newPos - 1) === '.' && config.currency) {
	newPos = newPos + 1;
      }
      state.selection = {start: newPos, end: newPos};
    } else {
      state.selection = {start: state.value.length - state.posFromEnd, end: state.value.length - state.posFromEnd};
    }
    if (config.currency) {
      state.value = addCommas(state.value, state.selection);
      state.value = addSignAndPadding(state.value, state.selection);
    }
  }

  function getState() {
    return {
      value: edit.value,
      selection: getSelection(),
      posFromEnd: -1,
      sign: false
    }
  }

  function setState(state) {
    edit.value = state.value;
    setSelection(state.selection.start, state.selection.end);
  }

  this.keyPressed = function(o, event) {
    event = jQuery.event.fix(event);
    event.preventDefault();
    handleChar(event.charCode || event.keyCode);
  }

  function handleChar(charCode) {
    if (charCode >= '0'.charCodeAt(0) && charCode <= '9'.charCodeAt(0)) {
      insertChar(charCode);
    } else if (config.allowNegative && (charCode === '-'.charCodeAt(0) || charCode === '+'.charCodeAt(0))) {
      var selection = getSelection();
      var sign = (charCode === '-'.charCodeAt(0));
      edit.value = insertSign(clearSign(edit.value), sign);
      setSelection(selection.start, selection.end);
    } else if (isDecimal() && charCode === '.'.charCodeAt(0)) {
      insertChar(charCode);
    }
  }

  // Insert a character: a . or 0-9
  function insertChar(charCode) {
    var state = getState();
    removeExtraStuff(state);
    var myChar = String.fromCharCode(charCode);
    if (state.selection.start !== state.selection.end) {
      state.value = clearRange(state.value, state.selection.start, state.selection.end);
    }
    dotPos = state.value.indexOf('.');
    if (myChar === '.' && dotPos !== -1) {
      // If there is already a dot in the field, don't do anything
      return;
    }
    if (state.value.length - (dotPos === -1 && myChar !== '.' ? 0 : 1) >= config.digits) {
      // If inserting the new value makes the field too long, don't do anything
      return;
    }
    state.value = state.value.substring(0, state.selection.start) + myChar +
      state.value.substring(state.selection.start);
    addExtraStuff(state, false);
    setState(state);
  }

  // Remove the selection according to the given keyCode.
  //
  // If the selection is empty (start === end), then backspace deletes
  // characters before the cursor, and delete deletes characters after
  // the cursor.
  //
  // If the selection is nonempty (start !== end) then the selection is
  // removed.
  function remove(keyCode) {
    var state = getState();
    if (keyCode === DELETE_KEYCODE && state.selection.start === state.selection.end &&
	state.value.charAt(state.selection.start) === ',') {
      state.selection.start ++;
      state.selection.end ++;
    }
    removeExtraStuff(state);
    if (state.selection.start !== state.selection.end) {
      state.value = clearRange(state.value, state.selection.start, state.selection.end);
    } else if (keyCode === BACKSPACE_KEYCODE) {
      state.value = clearRange(state.value, state.selection.start - 1, state.selection.start);
    } else if (keyCode === DELETE_KEYCODE) {
      state.value = clearRange(state.value, state.selection.start, state.selection.start + 1);
    }
    var moveRight = state.selection.start === state.selection.end && keyCode === DELETE_KEYCODE;
    addExtraStuff(state, moveRight);
    setState(state);
  }

  this.keyWentDown = function(o, event) {
    event = jQuery.event.fix(event);
    var selection = getSelection();
    if (event.keyCode === BACKSPACE_KEYCODE || event.keyCode === DELETE_KEYCODE) {
      event.preventDefault();
      remove(event.keyCode);
    } else if (event.keyCode === LEFT_KEYCODE) {
      if (selection.start === selection.end) {
	event.preventDefault();
	if (!(selection.start > 1 && (edit.value.charAt(selection.start - 1) === config.padChar.charAt(0) &&
		selection.start != edit.value.length ||
		edit.value.charAt(selection.start - 1) === config.currencySymbol.charAt(0)))) {
	  selection.start --;
	  selection.end --;
	  if (selection.start > 1 && (edit.value.charAt(selection.start - 1) === ',' ||
		edit.value.charAt(selection.start - 1) === '.' && config.currency)) {
	    selection.start --;
	    selection.end --;
	  }
	  setSelection(selection.start, selection.end);
	}
      }
    } else if (event.keyCode === RIGHT_KEYCODE) {
      if (selection.start === selection.end) {
	event.preventDefault();
	if (!(config.allowNegative && selection.start === edit.value.length - 1 &&
	    (edit.value.charAt(selection.start) === ' ' || edit.value.charAt(selection.start) === '-'))) {
	  if (selection.start < edit.value.length && edit.value.charAt(selection.start) === ',' ||
		edit.value.charAt(selection.start) === '.' && config.currency) {
	    selection.start ++;
	    selection.end ++;
	  }
	  selection.start ++;
	  selection.end ++;
	  setSelection(selection.start, selection.end);
	}
      }
    } else if (event.keyCode === HOME_KEYCODE) {
      event.preventDefault();
      var start = 0;
      if (config.currency) {
	while (start < edit.value.length && (edit.value.charAt(start) === config.currencySymbol.charAt(0) ||
	      edit.value.charAt(start) === config.padChar.charAt(0))) {
	  start ++;
	}
      }
      setSelection(start, start);
    } else if (event.keyCode === END_KEYCODE) {
      event.preventDefault();
      if (config.allowNegative) {
	setSelection(edit.value.length - 1, edit.value.length - 1);
      } else {
	setSelection(edit.value.length, edit.value.length);
      }
    }
  }

  function cut(event) {
    jQuery.event.fix(event).preventDefault();
    var selection = getSelection();
    if (selection.start !== selection.end) {
      var cutData = edit.value.substring(selection.start, selection.end);
      if (window.clipboardData && window.clipboardData.setData) {
	window.clipboardData.setData("Text", cutData);
      } else if (event.clipboardData && event.clipboardData.setData) {
	event.clipboardData.setData("text/plain", cutData);
      }
      remove();
    }
  }

  if (edit.addEventListener) {
    edit.addEventListener('cut', cut, false);
  } else if (edit.attachEvent) {
    edit.attachEvent("oncut", cut);
  }

  function paste(event) {
    jQuery.event.fix(event).preventDefault();
    var pasteData = undefined;
    if (window.clipboardData && window.clipboardData.getData) {
      pasteData = window.clipboardData.getData('Text');
    } else if (event.clipboardData && event.clipboardData.getData) {
      pasteData = event.clipboardData.getData('text/plain');
    } else {
      return;
    }
    // Insert the pasted data char by char.
    for (var i = 0; i < pasteData.length; i++) {
      handleChar(pasteData.charCodeAt(i));
    }
  }

  if (edit.addEventListener) {
    edit.addEventListener('paste', paste, false);
  } else if (edit.attachEvent) {
    edit.attachEvent('onpaste', paste);
  }

  function input(event) {
    // When the value was cleared, set it to 0.
    // e.g. IE does this when the X button is clicked.
    if (edit.value === "") {
      var state = {value: "0", selection:{start: Infinity, end: Infinity}, sign: false, posFromEnd: 0};
      addExtraStuff(state, false);
      setState(state);
    }
  }

  if (edit.addEventListener) {
    edit.addEventListener("input", input, false);
  } else if (edit.attachEvent) {
    edit.attachEvent("oninput", input);
  }
}
