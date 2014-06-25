#include <powadd.hpp>
#include <rptgen.hpp>
#include <misc.hpp>
#include <gui.hpp>
#include <guip.hpp>
#include <nrd.hpp>
#include <nrdp1.hpp>
#include <olh.hpp>
#include <mix.hpp>
#include <finscatter.hpp>
#include <boost/date_time/gregorian/greg_year.hpp>

using namespace Wt;
using namespace Wt::Chart;
using namespace Wt::Ext;


static int ScatterType=0;
static short DEFINITION_TYPE=0;
static char *FUND_CODE=NULL,*fromCategory=NULL,*toCategory=NULL;

static RDAfinmgt *fmgt=NULL;

struct BalSumScatterColumns {
	char *reference;
	double amt;
};
typedef struct BalSumScatterColumns BalSumScatterColumn;

struct BalSumScatters {
	char *period;
	int num;
	BalSumScatterColumn *C;
}; 
typedef struct BalSumScatters BalSumScatter;

static Wt::WContainerWidget *ActualBalSumScatter=NULL;
static Wt::WComboBox *FUND_COMBOBOX=NULL,*SCATTER_TYPE=NULL;
static APPlib *FINBREFS=NULL,*SCATTER_TYPES=NULL,*FUNDS=NULL;
static int cashFINBREF=0,expFINBREF=0,revFINBREF=0,fbuFINBREF=0;
static int numBalSumData=0;
static BalSumScatter *BalSumData=NULL;
static Wt::WContainerWidget *BSScatterWidget(short,char *);



PanelList::PanelList(WContainerWidget *parent)
  : WContainerWidget(parent)
{ }

WPanel *PanelList::addWidget(const WString& text, WWidget *w)
{
  WPanel *p = new WPanel();
  p->setTitle(text);
  p->setCentralWidget(w);

  addPanel(p);

  return p;
}

void PanelList::addPanel(WPanel *panel)
{
  panel->setCollapsible(true);
  panel->collapse();

  panel->expandedSS().connect(this, &PanelList::onExpand);

  WContainerWidget::addWidget(panel);
}

void PanelList::onExpand(bool notUndo)
{
  WPanel *panel = dynamic_cast<WPanel *>(sender());

  if (notUndo) {
    wasExpanded_ = -1;

    for (unsigned i = 0; i < children().size(); ++i) {
      WPanel *p = dynamic_cast<WPanel *>(children()[i]);
      if (p != panel) {
	if (!p->isCollapsed())
	  wasExpanded_ = i;
	p->collapse();
      }
    }
  } else {
    if (wasExpanded_ != -1) {
      WPanel *p = dynamic_cast<WPanel *>(children()[wasExpanded_]);
      p->expand();
    }
  }
}


namespace {
  void addHeader(WTable *t, const char *value) {
    t->elementAt(0, t->columnCount())->addWidget(new WText(value));
  }

  void addEntry(WAbstractItemModel *model, const char *value) {
    model->insertRows(model->rowCount(), 1);
    model->setData(model->rowCount()-1, 0, boost::any(std::string(value)));
  }

  bool getDouble(WLineEdit *edit, double& value) {
    try {
      value = WLocale::currentLocale().toDouble(edit->text());
      return true;
    } catch (...) {
      return false;
    }
  }

  int seriesIndexOf(WCartesianChart* chart, int modelColumn) {
    for (unsigned i = 0; i < chart->series().size(); ++i)
      if (chart->series()[i].modelColumn() == modelColumn)
	return i;
    
    return -1;
  }
}

ChartConfig::ChartConfig(WCartesianChart *chart, WContainerWidget *parent)
  : WContainerWidget(parent),
    chart_(chart),
    fill_(MinimumValueFill)
{
  chart_->setLegendStyle(chart_->legendFont(), WPen(black),
			 WBrush(WColor(0xFF, 0xFA, 0xE5)));
  chart->initLayout();

  PanelList *list = new PanelList(this);

  WIntValidator *sizeValidator = new WIntValidator(200, 2000, this);
  sizeValidator->setMandatory(true);

  WDoubleValidator *anyNumberValidator = new WDoubleValidator(this);
  anyNumberValidator->setMandatory(true);

  WDoubleValidator *angleValidator = new WDoubleValidator(-90, 90, this);
  angleValidator->setMandatory(true);

  // ---- Chart properties ----

  WStandardItemModel *orientation = new WStandardItemModel(0, 1, this);
  addEntry(orientation, "Vertical");
  addEntry(orientation, "Horizontal");

  WStandardItemModel *legendLocation = new WStandardItemModel(0, 1, this);
  addEntry(legendLocation, "Outside");
  addEntry(legendLocation, "Inside");

  WStandardItemModel *legendSide = new WStandardItemModel(0, 1, this);
  addEntry(legendSide, "Top");
  addEntry(legendSide, "Right");
  addEntry(legendSide, "Bottom");
  addEntry(legendSide, "Left");

  WStandardItemModel *legendAlignment = new WStandardItemModel(0, 1, this);
  addEntry(legendAlignment, "AlignLeft");
  addEntry(legendAlignment, "AlignCenter");
  addEntry(legendAlignment, "AlignRight");
  addEntry(legendAlignment, "AlignTop");
  addEntry(legendAlignment, "AlignMiddle");
  addEntry(legendAlignment, "AlignBottom");

  WTable *chartConfig = new WTable();
  chartConfig->setMargin(WLength::Auto, Left | Right);

  int row = 0;
  chartConfig->elementAt(row, 0)->addWidget(new WText("Title:"));
  titleEdit_ = new WLineEdit(chartConfig->elementAt(row, 1));
  connectSignals(titleEdit_);
  ++row;

  chartConfig->elementAt(row, 0)->addWidget(new WText("Width:"));
  chartWidthEdit_ = new WLineEdit(chartConfig->elementAt(row, 1));
  chartWidthEdit_
    ->setText(WLocale::currentLocale().toString(chart_->width().value()));
  chartWidthEdit_->setValidator(sizeValidator);
  chartWidthEdit_->setMaxLength(4);
  connectSignals(chartWidthEdit_);
  ++row;

  chartConfig->elementAt(row, 0)->addWidget(new WText("Height:"));
  chartHeightEdit_ = new WLineEdit(chartConfig->elementAt(row, 1));
  chartHeightEdit_
    ->setText(WLocale::currentLocale().toString(chart_->height().value()));
  chartHeightEdit_->setValidator(sizeValidator);
  chartHeightEdit_->setMaxLength(4);
  connectSignals(chartHeightEdit_);
  ++row;

  chartConfig->elementAt(row, 0)->addWidget(new WText("Orientation:"));
  chartOrientationEdit_ = new WComboBox(chartConfig->elementAt(row, 1));
  chartOrientationEdit_->setModel(orientation);
  connectSignals(chartOrientationEdit_);
  ++row;

  chartConfig->elementAt(row, 0)->addWidget(new WText("Legend location:"));
  legendLocationEdit_ = new WComboBox(chartConfig->elementAt(row, 1));
  legendLocationEdit_->setModel(legendLocation);
  connectSignals(legendLocationEdit_);
  ++row;

  chartConfig->elementAt(row, 0)->addWidget(new WText("Legend side:"));
  legendSideEdit_ = new WComboBox(chartConfig->elementAt(row, 1));
  legendSideEdit_->setModel(legendSide);
  legendSideEdit_->setCurrentIndex(1);
  connectSignals(legendSideEdit_);
  ++row;

  chartConfig->elementAt(row, 0)->addWidget(new WText("Legend alignment:"));
  legendAlignmentEdit_ = new WComboBox(chartConfig->elementAt(row, 1));
  legendAlignmentEdit_->setModel(legendAlignment);
  legendAlignmentEdit_->setCurrentIndex(4);
  connectSignals(legendAlignmentEdit_);
  ++row;

  for (int i = 0; i < chartConfig->rowCount(); ++i) {
    chartConfig->elementAt(i, 0)->setStyleClass("tdhead");
    chartConfig->elementAt(i, 1)->setStyleClass("tddata");
  }

  WPanel *p = list->addWidget("Chart properties", chartConfig);
  p->setMargin(WLength::Auto, Left | Right);
  p->resize(880, WLength::Auto);
  p->setMargin(20, Top | Bottom);

  if (chart_->isLegendEnabled())
    chart_->setPlotAreaPadding(200, Right);

  // ---- Series properties ----

  WStandardItemModel *types = new WStandardItemModel(0, 1, this);
  addEntry(types, "Points");
  addEntry(types, "Line");
  addEntry(types, "Curve");
  addEntry(types, "Bar");
  addEntry(types, "Line Area");
  addEntry(types, "Curve Area");
  addEntry(types, "Stacked Bar");
  addEntry(types, "Stacked Line Area");
  addEntry(types, "Stacked Curve Area");

  WStandardItemModel *markers = new WStandardItemModel(0, 1, this);
  addEntry(markers, "None");
  addEntry(markers, "Square");
  addEntry(markers, "Circle");
  addEntry(markers, "Cross");
  addEntry(markers, "X cross");
  addEntry(markers, "Triangle");
  addEntry(markers, "Diamond");

  WStandardItemModel *axes = new WStandardItemModel(0, 1, this);
  addEntry(axes, "1st Y axis");
  addEntry(axes, "2nd Y axis");

  WStandardItemModel *labels = new WStandardItemModel(0, 1, this);
  addEntry(labels, "None");
  addEntry(labels, "X");
  addEntry(labels, "Y");
  addEntry(labels, "X: Y");

  WTable *seriesConfig = new WTable();
  seriesConfig->setMargin(WLength::Auto, Left | Right);

  ::addHeader(seriesConfig, "Name");
  ::addHeader(seriesConfig, "Enabled");
  ::addHeader(seriesConfig, "Type");
  ::addHeader(seriesConfig, "Marker");
  ::addHeader(seriesConfig, "Y axis");
  ::addHeader(seriesConfig, "Legend");
  ::addHeader(seriesConfig, "Shadow");
  ::addHeader(seriesConfig, "Value labels");

  seriesConfig->rowAt(0)->setStyleClass("trhead");

  for (int j = 1; j < chart->model()->columnCount(); ++j) {
    SeriesControl sc;

    new WText(asString(chart->model()->headerData(j)),
	      seriesConfig->elementAt(j, 0));

    sc.enabledEdit = new WCheckBox(seriesConfig->elementAt(j, 1));
    connectSignals(sc.enabledEdit);

    sc.typeEdit = new WComboBox(seriesConfig->elementAt(j, 2));
    sc.typeEdit->setModel(types);
    connectSignals(sc.typeEdit);

    sc.markerEdit = new WComboBox(seriesConfig->elementAt(j, 3));
    sc.markerEdit->setModel(markers);
    connectSignals(sc.markerEdit);

    sc.axisEdit = new WComboBox(seriesConfig->elementAt(j, 4));
    sc.axisEdit->setModel(axes);
    connectSignals(sc.axisEdit);

    sc.legendEdit = new WCheckBox(seriesConfig->elementAt(j, 5));
    connectSignals(sc.legendEdit);

    sc.shadowEdit = new WCheckBox(seriesConfig->elementAt(j, 6));
    connectSignals(sc.shadowEdit);

    sc.labelsEdit = new WComboBox(seriesConfig->elementAt(j, 7));
    sc.labelsEdit->setModel(labels);
    connectSignals(sc.labelsEdit);

    int si = seriesIndexOf(chart, j);

    if (si != -1) {
      sc.enabledEdit->setChecked();
      const WDataSeries& s = chart_->series(j);
      switch (s.type()) {
      case PointSeries:
	sc.typeEdit->setCurrentIndex(0); break;
      case LineSeries:
	sc.typeEdit->setCurrentIndex(s.fillRange() != NoFill ?
				     (s.isStacked() ? 7 : 4) : 1); break;
      case CurveSeries:
	sc.typeEdit->setCurrentIndex(s.fillRange() != NoFill ?
				     (s.isStacked() ? 8 : 5) : 2); break;
      case BarSeries:
	sc.typeEdit->setCurrentIndex(s.isStacked() ? 6 : 3);
      }

      sc.markerEdit->setCurrentIndex((int)s.marker());
      sc.legendEdit->setChecked(s.isLegendEnabled());
      sc.shadowEdit->setChecked(s.shadow() != WShadow());
    }

    seriesControls_.push_back(sc);

    seriesConfig->rowAt(j)->setStyleClass("trdata");
  }

  p = list->addWidget("Series properties", seriesConfig);
  p->setMargin(WLength::Auto, Left | Right);
  p->resize(880, WLength::Auto);
  p->setMargin(20, Top | Bottom);

  // ---- Axis properties ----

  WStandardItemModel *yScales = new WStandardItemModel(0, 1, this);
  addEntry(yScales, "Linear scale");
  addEntry(yScales, "Log scale");

  WStandardItemModel *xScales = new WStandardItemModel(0, 1, this);
  addEntry(xScales, "Categories");
  addEntry(xScales, "Linear scale");
  addEntry(xScales, "Log scale");
  addEntry(xScales, "Date scale");

  WTable *axisConfig = new WTable();
  axisConfig->setMargin(WLength::Auto, Left | Right);

  ::addHeader(axisConfig, "Axis");
  ::addHeader(axisConfig, "Visible");
  ::addHeader(axisConfig, "Scale");
/*
  ::addHeader(axisConfig, "Automatic");
  ::addHeader(axisConfig, "Minimum");
  ::addHeader(axisConfig, "Maximum");
*/
  ::addHeader(axisConfig, "Gridlines");
  ::addHeader(axisConfig, "Label angle");

  axisConfig->rowAt(0)->setStyleClass("trhead");

  for (int i = 0; i < 3; ++i) {
    const char *axisName[] = { "X axis", "1st Y axis", "2nd Y axis" };
    int j = i + 1;

    const WAxis& axis = chart_->axis(static_cast<Axis>(i));
    AxisControl sc;

    new WText(WString(axisName[i], UTF8), axisConfig->elementAt(j, 0));

    sc.visibleEdit = new WCheckBox(axisConfig->elementAt(j, 1));
    sc.visibleEdit->setChecked(axis.isVisible());
    connectSignals(sc.visibleEdit);

    sc.scaleEdit = new WComboBox(axisConfig->elementAt(j, 2));
    if (axis.scale() == CategoryScale)
      sc.scaleEdit->addItem("Category scale");
    else {
      if (axis.id() == XAxis) {
	sc.scaleEdit->setModel(xScales);
	sc.scaleEdit->setCurrentIndex(axis.scale()-1);
      } else {
	sc.scaleEdit->setModel(yScales);
	sc.scaleEdit->setCurrentIndex(axis.scale() - 1);
      }
    }
    connectSignals(sc.scaleEdit);

/*
    bool autoValues = axis.autoLimits() == (MinimumValue | MaximumValue);

    sc.minimumEdit = new WLineEdit(axisConfig->elementAt(j, 4));
    sc.minimumEdit->setText(WLocale::currentLocale()
                            .toString(axis.minimum()));
    sc.minimumEdit->setValidator(anyNumberValidator);
    sc.minimumEdit->setEnabled(!autoValues);
    connectSignals(sc.minimumEdit);

    sc.maximumEdit = new WLineEdit(axisConfig->elementAt(j, 5));
    sc.maximumEdit->setText(WLocale::currentLocale()
                            .toString(axis.maximum()));
    sc.maximumEdit->setValidator(anyNumberValidator);
    sc.maximumEdit->setEnabled(!autoValues);
    connectSignals(sc.maximumEdit);

    sc.autoEdit = new WCheckBox(axisConfig->elementAt(j, 3));
    sc.autoEdit->setChecked(autoValues);
    sc.autoEdit->setChecked(TRUE);
    connectSignals(sc.autoEdit);
    sc.autoEdit->checked().connect(sc.maximumEdit, &WLineEdit::disable);
    sc.autoEdit->unChecked().connect(sc.maximumEdit, &WLineEdit::enable);
    sc.autoEdit->checked().connect(sc.minimumEdit, &WLineEdit::disable);
    sc.autoEdit->unChecked().connect(sc.minimumEdit, &WLineEdit::enable);
*/

    sc.gridLinesEdit = new WCheckBox(axisConfig->elementAt(j, 6));
    connectSignals(sc.gridLinesEdit);

    sc.labelAngleEdit = new WLineEdit(axisConfig->elementAt(j, 7));
    sc.labelAngleEdit->setText("0");
    sc.labelAngleEdit->setValidator(angleValidator);
    connectSignals(sc.labelAngleEdit);

    axisConfig->rowAt(j)->setStyleClass("trdata");

    axisControls_.push_back(sc);
  }

  p = list->addWidget("Axis properties", axisConfig);
  p->setMargin(WLength::Auto, Left | Right);
  p->resize(880, WLength::Auto);
  p->setMargin(20, Top | Bottom);

  /*
   * If we do not have JavaScript, then add a button to reflect changes to
   * the chart.
   */
  if (!WApplication::instance()->environment().javaScript()) {
    WPushButton *b = new WPushButton(this);
    b->setText("Update chart");
    b->setInline(false); // so we can add margin to center horizontally
    b->setMargin(WLength::Auto, Left | Right);
    b->clicked().connect(this, &ChartConfig::update);
  }
}

void ChartConfig::setValueFill(FillRangeType fill)
{
  fill_ = fill;
}

void ChartConfig::update()
{
  bool haveLegend = false;
  std::vector<WDataSeries> series;

  for (int i = 1; i < chart_->model()->columnCount(); ++i) {
    SeriesControl& sc = seriesControls_[i-1];

    if (sc.enabledEdit->isChecked()) {
      WDataSeries s(i);

      switch (sc.typeEdit->currentIndex()) {
      case 0:
	s.setType(PointSeries);
	if (sc.markerEdit->currentIndex() == 0)
	  sc.markerEdit->setCurrentIndex(1);
	break;
      case 1:
	s.setType(LineSeries);
	sc.markerEdit->setCurrentIndex(0);
	break;
      case 2:
	s.setType(CurveSeries);
	sc.markerEdit->setCurrentIndex(0);
	break;
      case 3:
	s.setType(BarSeries);
	sc.markerEdit->setCurrentIndex(0);
	break;
      case 4:
	s.setType(LineSeries);
	s.setFillRange(fill_);
	sc.markerEdit->setCurrentIndex(0);
	break;
      case 5:
	s.setType(CurveSeries);
	s.setFillRange(fill_);
	sc.markerEdit->setCurrentIndex(0);
	break;
      case 6:
	s.setType(BarSeries);
	s.setStacked(true);
	sc.markerEdit->setCurrentIndex(0);
	break;
      case 7:
	s.setType(LineSeries);
	s.setFillRange(fill_);
	s.setStacked(true);
	sc.markerEdit->setCurrentIndex(0);
	break;
      case 8:
	s.setType(CurveSeries);
	s.setFillRange(fill_);
	s.setStacked(true);
	sc.markerEdit->setCurrentIndex(0);
      }

      //set WPainterPath to draw a diamond
      if(sc.markerEdit->currentIndex() == CustomMarker){
	WPainterPath pp = WPainterPath();
	pp.moveTo(-6, 0);
	pp.lineTo(0, 6);
	pp.lineTo(6, 0);
	pp.lineTo(0, -6);
	pp.lineTo(-6, 0);
	s.setCustomMarker(pp);
      }

      s.setMarker(static_cast<MarkerType>(sc.markerEdit->currentIndex()));

      if (sc.axisEdit->currentIndex() == 1) {
	s.bindToAxis(Y2Axis);
      }

      if (sc.legendEdit->isChecked()) {
	s.setLegendEnabled(true);
	haveLegend = true;
      } else
	s.setLegendEnabled(false);

      if (sc.shadowEdit->isChecked()) {
	s.setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3));
      } else
	s.setShadow(WShadow());

      switch (sc.labelsEdit->currentIndex()) {
      case 1:
	s.setLabelsEnabled(XAxis);
	break;
      case 2:
	s.setLabelsEnabled(YAxis);
	break;
      case 3:
	s.setLabelsEnabled(XAxis);	
	s.setLabelsEnabled(YAxis);
	break;
      }

      series.push_back(s);
    }
  }

  chart_->setSeries(series);

  for (int i = 0; i < 3; ++i) {
    AxisControl& sc = axisControls_[i];
    WAxis& axis = chart_->axis(static_cast<Axis>(i));

    axis.setVisible(sc.visibleEdit->isChecked());

    if (sc.scaleEdit->count() != 1) {
      int k = sc.scaleEdit->currentIndex();
      if (axis.id() != XAxis)
	k += 1;
      else {
	if (k == 0)
	  chart_->setType(CategoryChart);
	else
	  chart_->setType(ScatterPlot);
      }

      switch (k) {
      case 1:
	axis.setScale(LinearScale); break;
      case 2:
	axis.setScale(LogScale); break;
      case 3:
	axis.setScale(DateScale); break;
      }
    }

/*
    if (sc.autoEdit->isChecked())
      axis.setAutoLimits(MinimumValue | MaximumValue);
    else {
      if (validate(sc.minimumEdit) && validate(sc.maximumEdit)) {
          double min, max;
          getDouble(sc.minimumEdit, min);
          getDouble(sc.maximumEdit, max);

          if (axis.scale() == LogScale)
              if (min <= 0)
                  min = 0.0001;

          if (axis.scale() == DateScale){
              //the number of julian days until year 1986
              WDate dMin = WDate(1900,1,1);
              double gregDaysMin = (double)dMin.toJulianDay();
              //the number of julian days until year 1988
              WDate dMax = WDate(3000,1,1);
              double gregDaysMax = (double)dMax.toJulianDay();

              bool greg_year_validation =
                      (min > gregDaysMin &&
                       min < gregDaysMax &&
                       max > gregDaysMin &&
                       max < gregDaysMax);

              if(!greg_year_validation){
                  min = gregDaysMin;
                  max = gregDaysMax;
              }
          }

          axis.setRange(min, max);
      }

    }
*/

    if (validate(sc.labelAngleEdit)) {
      double angle;
      getDouble(sc.labelAngleEdit, angle);
      axis.setLabelAngle(angle);
    }

    axis.setGridLinesEnabled(sc.gridLinesEdit->isChecked());
  }

  chart_->setTitle(titleEdit_->text());

  if (validate(chartWidthEdit_) && validate(chartHeightEdit_)) {
    double width, height;
    getDouble(chartWidthEdit_, width);
    getDouble(chartHeightEdit_, height);
    chart_->resize(width, height);
  }

  switch (chartOrientationEdit_->currentIndex()) {
  case 0:
    chart_->setOrientation(Vertical); break;
  case 1:
    chart_->setOrientation(Horizontal); break;
  }

  chart_->setLegendEnabled(haveLegend);

  if (haveLegend) {
    LegendLocation location = LegendOutside;
    Side side = Right;
    AlignmentFlag alignment = AlignMiddle;

    switch (legendLocationEdit_->currentIndex()) {
    case 0: location = LegendOutside; break;
    case 1: location = LegendInside; break;
    }

    switch (legendSideEdit_->currentIndex()) {
    case 0: side = Top; break;
    case 1: side = Right; break;
    case 2: side = Bottom; break;
    case 3: side = Left; break;
    }

    if (side == Left || side == Right) {
      if (legendAlignmentEdit_->currentIndex() < 3)
	legendAlignmentEdit_->setCurrentIndex(4);
    } else {
      if (legendAlignmentEdit_->currentIndex() >= 3)
	legendAlignmentEdit_->setCurrentIndex(2);
    }

    switch (legendAlignmentEdit_->currentIndex()) {
    case 0: alignment = AlignLeft; break;
    case 1: alignment = AlignCenter; break;
    case 2: alignment = AlignRight; break;
    case 3: alignment = AlignTop; break;
    case 4: alignment = AlignMiddle; break;
    case 5: alignment = AlignBottom; break;
    }

    chart_->setLegendLocation(location, side, alignment);

    chart_->setLegendColumns((side == Top || side == Bottom ) ? 2 : 1,
			     WLength(100));
  }

  for (unsigned i = 0; i < 4; ++i) {
    Side sides[] = { Top, Right, Bottom, Left };

    bool legendRoom =
      haveLegend
      && chart_->legendLocation() == LegendOutside
      && chart_->legendSide() == sides[i];

    int padding;

    if (i % 2 == 0)
      padding = legendRoom ? 80 : 40;
    else
      padding = legendRoom ? 200 : 80;

    chart_->setPlotAreaPadding(padding, sides[i]);
  }
}

bool ChartConfig::validate(WFormWidget *w)
{
  bool valid = w->validate() == WValidator::Valid;

  if (!WApplication::instance()->environment().javaScript()) {
    w->setStyleClass(valid ? "" : "Wt-invalid");
    w->setToolTip(valid ? "" : "Invalid value");
  }

  return valid;
}

void ChartConfig::connectSignals(WFormWidget *w)
{
  w->changed().connect(this, &ChartConfig::update);
  if (dynamic_cast<WLineEdit *>(w))
    w->enterPressed().connect(this, &ChartConfig::update);
}

void FreeBalSumData()
{
	int x=0,y=0;
	BalSumScatterColumn *c=NULL;
	BalSumScatter *b=NULL;

	if(BalSumData!=NULL)
	{
		for(x=0,b=BalSumData;x<numBalSumData;++x,++b)
		{
			if(b->period!=NULL) Rfree(b->period);
			if(b->C!=NULL)
			{
				for(y=0,c=b->C;y<b->num;++y,++c)
				{
					if(c->reference!=NULL) Rfree(c->reference);
				}
				Rfree(b->C);
				b->C=NULL;
				b->num=0;
			}
		}
		Rfree(BalSumData);
		BalSumData=NULL;
		numBalSumData=0;
	}
}
void AddBalSumData(char *period,char *reference,double amt)
{
	BalSumScatter *b=NULL;
	BalSumScatterColumn *c=NULL;
	char delflag=FALSE;
	short ef=0;
	int x=0,y=0;

	if(fromCategory==NULL)
	{
		fromCategory=stralloc(period);
	}
	if(toCategory!=NULL) Rfree(toCategory);
	toCategory=stralloc(period);
	if(BalSumData==NULL)
	{
		BalSumData=Rmalloc(sizeof(BalSumScatter));
		numBalSumData=1;
		b=BalSumData;
		b->period=stralloc(period);
		b->num=FINBREFS->numlibs;
		b->C=Rmalloc(FINBREFS->numlibs*sizeof(BalSumScatterColumn));
		if(ScatterType==0)
		{
			c=b->C;
			c->amt=0.0;
			c->reference=stralloc("CASH");
			if(!RDAstrcmp(c->reference,reference)) c->amt+=amt;
			++c;
			c->amt=0.0;
			c->reference=stralloc("FUND BALANCE-UNRESERVED");
			if(!RDAstrcmp(c->reference,reference)) c->amt+=amt;
			++c;
			c->amt=0.0;
			c->reference=stralloc("EXPENDITURE");
			if(!RDAstrcmp(c->reference,reference)) c->amt+=amt;
			++c;
			c->amt=0.0;
			c->reference=stralloc("REVENUE");
			if(!RDAstrcmp(c->reference,reference)) c->amt+=amt;
		} else {
			for(y=0;y<FINBREFS->numlibs;++y)
			{
				c=b->C+y;
				c->amt=0.0;
				c->reference=stralloc(FINBREFS->libs[y]);
				if(!RDAstrcmp(c->reference,reference)) c->amt+=amt;
			}
		}
	} else {
		for(x=0,b=BalSumData;x<numBalSumData;++x,++b)
		{
			if(!RDAstrcmp(b->period,period))
			{
				break;
			}
		}
		if(x<numBalSumData)
		{
			b=BalSumData+x;
			if(b->C!=NULL)
			{
				for(y=0,c=b->C;y<b->num;++y,++c)
				{
					if(!RDAstrcmp(c->reference,reference)) 
					{
						c->amt+=amt;
						break;
					}
				}
			}
		} else {
			BalSumData=Rrealloc(BalSumData,(numBalSumData+1)*sizeof(BalSumScatter));
			b=BalSumData+numBalSumData;
			b->period=stralloc(period);
			b->num=FINBREFS->numlibs;
			b->C=Rmalloc(FINBREFS->numlibs*sizeof(BalSumScatterColumn));
			if(ScatterType==0)
			{
				c=b->C;
				c->reference=stralloc("CASH");
				if(!RDAstrcmp(c->reference,reference)) c->amt+=amt;
				++c;
				c->reference=stralloc("FUND BALANCE-UNRESERVED");
				if(!RDAstrcmp(c->reference,reference)) c->amt+=amt;
				++c;
				c->reference=stralloc("EXPENDITURE");
				if(!RDAstrcmp(c->reference,reference)) c->amt+=amt;
				++c;
				c->reference=stralloc("REVENUE");
				if(!RDAstrcmp(c->reference,reference)) c->amt+=amt;
			} else {
				for(y=0;y<FINBREFS->numlibs;++y)
				{
					c=b->C+y;
					c->amt=0.0;
					c->reference=stralloc(FINBREFS->libs[y]);
					if(!RDAstrcmp(c->reference,reference)) c->amt+=amt;
				}
			}
			++numBalSumData;
		}
	}	
}
void GetBalSumData(short dtype,char *fund)
{
	int fiscal_year=0,x=0,MIN_Fiscal_Year=0,z=0;
	short finbyr=(-1),finbacc=(-1),finbref=(-1),factor=1,finbref_type=0;
	short ef=0,dtype2=0,use_total=FALSE;
	char active1=FALSE,delflag1=FALSE,delflag2=FALSE,*acct=NULL,*refid=NULL;
	char period[8],tempx[128],tempy[128];
	double amt1=0.0,amt2=0.0,total=0.0;
	char *libx=NULL;
	RDAGenericSetup *gsv=NULL;
	
	if(BalSumData!=NULL) FreeBalSumData();
	finbyr=OPNNRD("FINMGT","FINBYR");
	finbacc=OPNNRD("FINMGT","FINBACC");
	finbref=OPNNRD("FINMGT","FINBREF");
	if(finbacc==(-1) || finbyr==(-1) || finbref==(-1)) return;
	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libx,"%s/rda/FINMGT.GSV",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\FINMGT.GSV",CURRENTDIRECTORY);
#endif

	gsv=RDAGenericSetupNew("FINMGT","CURRENT FISCAL YEAR");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			MIN_Fiscal_Year=*gsv->value.short_value-1;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(FINBREFS!=NULL) freeapplib(FINBREFS);
	FINBREFS=APPlibNEW();
	if(ScatterType==1)
	{
		ZERNRD(finbacc);
		FINDFLDSETSHORT(finbacc,"DEFINITION TYPE",dtype);
		FINDFLDSETSTRING(finbacc,"ACCOUNT CODE",fund);
		ef=GTENRD(finbacc,1);
		while(!ef)
		{
			FINDFLDGETSHORT(finbacc,"DEFINITION TYPE",&dtype2);
			FINDFLDGETSTRING(finbacc,"ACCOUNT CODE",&acct);
			if(dtype!=dtype2 || strncmp(acct,fund,RDAstrlen(fund))) break;
			FINDFLDGETCHAR(finbacc,"DELETEFLAG",&delflag1);
			FINDFLDGETCHAR(finbacc,"ACTIVE",&active1);
			if(!delflag1) 
			{
				FINDFLDGETSTRING(finbacc,"REFERENCE TYPE",&refid);
				ZERNRD(finbref);
				FINDFLDSETSTRING(finbref,"REFERENCE TYPE",refid);
				if(!EQLNRD(finbref,1))
				{
					FINDFLDGETCHAR(finbref,"DELETEFLAG",&delflag2);
					if(!delflag2)
					{
						z=addAPPlibNoDuplicates(FINBREFS,refid);
						if(!RDAstrcmp(refid,"CASH")) cashFINBREF=z-1;	
						else if(!RDAstrcmp(refid,"EXPENDITURE")) expFINBREF=z-1;
						else if(!RDAstrcmp(refid,"REVENUE")) revFINBREF=z-1;
						else if(!RDAstrcmp(refid,"FUND BALANCE-UNRESERVED")) fbuFINBREF=z-1;
					}
				}
			}
			ef=NXTNRD(finbacc,1);
		}
		if(acct!=NULL) Rfree(acct);
		if(refid!=NULL) Rfree(refid);
	} else {
		addAPPlib(FINBREFS,"CASH");
		addAPPlib(FINBREFS,"FUND BALANCE-UNRESERVED");
		addAPPlib(FINBREFS,"EXPENDITURE");
		addAPPlib(FINBREFS,"REVENUE");
		cashFINBREF=0;
		fbuFINBREF=1;
		expFINBREF=2;
		revFINBREF=3;
	}
	fmgt=RDAfinmgtNEW();
	getfinmgtbin(fmgt);
	ZERNRD(finbyr);
	FINDFLDSETINT(finbyr,"FISCAL YEAR",MIN_Fiscal_Year);
	FINDFLDSETSHORT(finbyr,"DEFINITION TYPE",dtype);	
	FINDFLDSETSTRING(finbyr,"ACCOUNT CODE",fund);
	ef=GTENRD(finbyr,3);
	while(!ef)
	{
		FINDFLDGETINT(finbyr,"FISCAL YEAR",&fiscal_year);
		FINDFLDGETSHORT(finbyr,"DEFINITION TYPE",&dtype2);
		FINDFLDGETSTRING(finbyr,"ACCOUNT CODE",&acct);
		if(MIN_Fiscal_Year<fiscal_year || dtype!=dtype2) break;
		FINDFLDGETCHAR(finbyr,"DELETEFLAG",&delflag1);
		if(!delflag1 && !strncmp(acct,fund,RDAstrlen(fund))) 
		{
			ZERNRD(finbacc);
			COPYFIELD(finbyr,finbacc,"DEFINITION TYPE");
			COPYFIELD(finbyr,finbacc,"ACCOUNT CODE");
			if(EQLNRD(finbacc,1)) 
			{
				KEYNRD(finbacc,1);
			} else {
				FINDFLDGETSTRING(finbacc,"REFERENCE TYPE",&refid);
				if(ScatterType!=0 || (!RDAstrcmp(refid,"CASH") || 
					!RDAstrcmp(refid,"FUND BALANCE-UNRESERVED") || 
					!RDAstrcmp(refid,"EXPENDITURE") ||
					!RDAstrcmp(refid,"REVENUE")))
				{
					ZERNRD(finbref);
					FINDFLDSETSTRING(finbref,"REFERENCE TYPE",refid);
					if(EQLNRD(finbref,1))
					{
						KEYNRD(finbref,1);
					} else {
						FINDFLDGETSHORT(finbref,"TYPE",&finbref_type);
						if((!strncmp(refid,"FUND BALANCE",12) || !RDAstrcmp(refid,"REVENUE") || (finbref_type>0 && finbref_type<8)) && (RDAstrcmp(refid,"EXPENDITURE") && RDAstrcmp(refid,"ENCUMBRANCE") && RDAstrcmp(refid,"ESTIMATED REVENUE"))) factor=(-1);
							else factor=1;
						if(!RDAstrcmp(refid,"EXPENDITURE") || !RDAstrcmp(refid,"REVENUE") || !RDAstrcmp(refid,"ENCUMBRANCE")) use_total=FALSE;
							else use_total=TRUE;
						total=0;
						FINDFLDGETDOUBLE(finbyr,"BEGINNING BALANCE DEBITS",&amt1);
						FINDFLDGETDOUBLE(finbyr,"BEGINNING BALANCE CREDITS",&amt2);
						total+=(amt1-amt2);
						FINDFLDGETDOUBLE(finbyr,"PRE-YEAR DEBITS",&amt1);
						FINDFLDGETDOUBLE(finbyr,"PRE-YEAR CREDITS",&amt2);
						total+=(amt1-amt2);
						if((total)!=0) 
						{
							memset(period,0,8);
							sprintf(period,"%04d-00",fiscal_year);
							AddBalSumData(period,refid,factor*(total)/100);	
						}
						for(x=fmgt->month+1;x<13;++x)
						{
							memset(tempx,0,128);
							sprintf(tempx,"%s DEBITS",FISCALPERIOD[x]);
							FINDFLDGETDOUBLE(finbyr,tempx,&amt1);
							memset(tempx,0,128);
							sprintf(tempx,"%s CREDITS",FISCALPERIOD[x]);
							FINDFLDGETDOUBLE(finbyr,tempx,&amt2);
							total+=(amt1-amt2);
							if(use_total==TRUE) 
							{
								if((total)!=0)
								{
									memset(period,0,8);
									sprintf(period,"%04d-%02d",fiscal_year,x);
									AddBalSumData(period,refid,factor*(total)/100);
								}
							} else {
								if((amt1-amt2)!=0)
								{
									memset(period,0,8);
									sprintf(period,"%04d-%02d",fiscal_year,x);
									AddBalSumData(period,refid,factor*(amt1-amt2)/100);
								}
							}
						}
						for(x=1;x<fmgt->month+1;++x)
						{
							memset(tempx,0,128);
							sprintf(tempx,"%s DEBITS",FISCALPERIOD[x]);
							FINDFLDGETDOUBLE(finbyr,tempx,&amt1);
							memset(tempx,0,128);
							sprintf(tempx,"%s CREDITS",FISCALPERIOD[x]);
							FINDFLDGETDOUBLE(finbyr,tempx,&amt2);
							if((amt1-amt2)!=0)
							{
								memset(period,0,8);
								sprintf(period,"%04d-%02d",fiscal_year,x);
								AddBalSumData(period,refid,factor*(amt1-amt2)/100);
							}
							total+=(amt1-amt2);
							if(use_total==TRUE) 
							{
								if((total)!=0)
								{
									memset(period,0,8);
									sprintf(period,"%04d-%02d",fiscal_year,x);
									AddBalSumData(period,refid,factor*(total)/100);
								}
							} else {
								if((amt1-amt2)!=0)
								{
									memset(period,0,8);
									sprintf(period,"%04d-%02d",fiscal_year,x);
									AddBalSumData(period,refid,factor*(amt1-amt2)/100);
								}
							}
						}
						FINDFLDGETDOUBLE(finbyr,"POST YEAR DEBITS",&amt1);
						FINDFLDGETDOUBLE(finbyr,"POST YEAR CREDITS",&amt2);
						total+=(amt1-amt2);
						if(use_total==TRUE) 
						{
							if((total)!=0)
							{
								memset(period,0,8);
								sprintf(period,"%04d-%02d",fiscal_year,x);
								AddBalSumData(period,refid,factor*(total)/100);
							}
						} else {
							if((amt1-amt2)!=0)
							{
								memset(period,0,8);
								sprintf(period,"%04d-%02d",fiscal_year,x);
								AddBalSumData(period,refid,factor*(amt1-amt2)/100);
							}
						}
					}
				}
			}
		}
		ef=NXTNRD(finbyr,3);
	}	
	if(fmgt!=NULL) free_finmgt(fmgt);
	if(acct!=NULL) Rfree(acct);
	if(refid!=NULL) Rfree(refid);
	if(finbacc!=(-1)) CLSNRD(finbacc);
	if(finbref!=(-1)) CLSNRD(finbref);
	if(finbyr!=(-1)) CLSNRD(finbyr);
	return;
}
static APPlib *GETFUNDS(short dtype1,char *fund)
{
	short ef=0,fileno=(-1),atype1=2,atype2=2,dx1=0,dx2=0,dtype2=0;
	char *temp=NULL,active=FALSE,delflag=FALSE;
	APPlib *FUNDS=NULL;

	fileno=OPNNRD("FINMGT","FINCOA");
	if(fileno!=(-1))
	{
		atype1=2;
		dx1=0;
		ZERNRD(fileno);
		FINDFLDSETSHORT(fileno,"ACCOUNT TYPE",atype1);
		FINDFLDSETSHORT(fileno,"DEFINITION TYPE",dtype1);
		FINDFLDSETSHORT(fileno,"DIMENSION",dx1);
		FINDFLDSETSTRING(fileno,"NAME",fund);
		ef=GTENRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETSHORT(fileno,"ACCOUNT TYPE",&atype2);
			FINDFLDGETSHORT(fileno,"DIMENSION",&dx2);
			FINDFLDGETSHORT(fileno,"DEFINITION TYPE",&dtype2);
			if(atype1!=atype2 || dtype1!=dtype2 || dx1!=dx2) break;
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"ACTIVE",&active);
			if(!delflag && active)
			{
				FINDFLDGETSTRING(fileno,"NAME",&temp);
				if(!isEMPTY(temp))
				{
					if(FUNDS==NULL) FUNDS=APPlibNEW();
					addAPPlib(FUNDS,temp);
				}
			}
			ef=NXTNRD(fileno,1);
		}
		CLSNRD(fileno);
	}
	return(FUNDS);
}
static char *FundDescription(short dtype,char *fund)
{
	short fileno=(-1),atype=2,dx=0;
	char *temp=NULL;

	fileno=OPNNRD("FINMGT","FINCOA");
	if(fileno!=(-1))
	{
		atype=2;
		dx=0;
		ZERNRD(fileno);
		FINDFLDSETSHORT(fileno,"ACCOUNT TYPE",atype);
		FINDFLDSETSHORT(fileno,"DEFINITION TYPE",dtype);
		FINDFLDSETSHORT(fileno,"DIMENSION",dx);
		FINDFLDSETSTRING(fileno,"NAME",fund);
		if(!EQLNRD(fileno,1)) 
		{
			FINDFLDGETSTRING(fileno,"DESCRIPTION",&temp);
		}
		CLSNRD(fileno);
	}
	return(temp);
}
char *DetermineFund(short dtype,char *cfd)
{
	short fileno=(-1),ef=0,atype=2,dim=0;
	char *fund=NULL,delflag=FALSE,active1=FALSE,*temp=NULL;

	fileno=OPNNRD("FINMGT","FINFND");
	if(fileno!=(-1))
	{
		ZERNRD(fileno);
		if(!isEMPTY(cfd)) 
		{	
			FINDFLDSETSTRING(fileno,"FUND",cfd);
			ef=NXTNRD(fileno,1);
		} else {
			ef=FRSNRD(fileno,1);
		}
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			if(!delflag)
			{
				FINDFLDGETSTRING(fileno,"FUND",&fund);
				break;
			}
			ef=NXTNRD(fileno,1);
		}
		CLSNRD(fileno);
	}
	if(!isEMPTY(fund)) return(fund);
	fileno=OPNNRD("FINMGT","FINCOA");
	if(fileno!=(-1))
	{
		atype=2;
		dim=0;
		ZERNRD(fileno);
		FINDFLDSETSHORT(fileno,"ACCOUNT TYPE",atype);
		FINDFLDSETSHORT(fileno,"DEFINITION TYPE",dtype);
		FINDFLDSETSHORT(fileno,"DIMENSION",&dim);
		ef=GTENRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETSHORT(fileno,"ACCOUNT TYPE",atype);
			FINDFLDGETSHORT(fileno,"DIMENSION",&dim);
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			if(atype==2 && dim==0 && delflag==FALSE) 
			{
				FINDFLDGETSTRING(fileno,"NAME",&fund);
				break;
			}
			ZERNRD(fileno);
			atype=2;
			dim=0;
			ef=GTENRD(fileno,1);
		}
		CLSNRD(fileno);
	}
	if(!isEMPTY(fund)) return(fund);
	fileno=OPNNRD("FINMGT","FINBACC");
	if(fileno!=(-1))
	{
		ZERNRD(fileno);
		if(dtype!=0 || !isEMPTY(cfd))
		{
			FINDFLDSETSHORT(fileno,"DEFINITION TYPE",dtype);
			FINDFLDSETSTRING(fileno,"ACCOUNT CODE",cfd);
			ef=GTENRD(fileno,1);
		} else {
			ef=FRSNRD(fileno,1);
		}
		while(!ef)
		{
			FINDFLDGETCHAR(fileno,"DELETEFLAG",&delflag);
			FINDFLDGETCHAR(fileno,"ACTIVE",&active1);
			if(!delflag && active1)
			{
				FINDFLDGETSTRING(fileno,"ACCOUNT CODE",&fund);
				for(temp=fund;*temp;++temp)
				{
					if(*temp==' ' || *temp=='-' || *temp=='/' || *temp=='.' || *temp==':') 
					{
						*temp=0;
						break;
					}
				}
				if(!isEMPTY(fund)) break;
			}
			ef=NXTNRD(fileno,1);
		}
		CLSNRD(fileno);		
	}
	return(fund);
}
short DetermineDEFTYPE(char *fd)
{
	short fileno=(-1),ef=FALSE,atype=2,dim=0,dtype=0,DefType=(-1);
	char delflag,*name=NULL;

	fileno=OPNNRD("FINMGT","FINCOA");
	if(fileno!=(-1))
	{
		while(ef)
		{
			ZERNRD(fileno);
			FINDFLDSETSHORT(fileno,"ACCOUNT TYPE",atype);
			FINDFLDSETSHORT(fileno,"DEFINITION TYPE",dtype);
			FINDFLDSETSHORT(fileno,"DIMENSION",dim);
			FINDFLDSETSTRING(fileno,"NAME",fd);
			ef=EQLNRD(fileno,1);
			if(!ef)
			{
				DefType=dtype;
				break;
			}
			++dtype;
		}
		CLSNRD(fileno);
	}
	if(DefType!=(-1)) return(DefType);
	dtype=0;
	fileno=OPNNRD("FINMGT","FINBACC");
	if(fileno!=(-1)) 
	{
		ZERNRD(fileno);
		FINDFLDSETSHORT(fileno,"DEFINITION TYPE",dtype);
		FINDFLDSETSTRING(fileno,"ACCOUNT CODE",fd);
		ef=GTENRD(fileno,1);
		while(!ef)
		{
			FINDFLDGETSTRING(fileno,"ACCOUNT CODE",&name);
			if(!strncmp(name,fd,RDAstrlen(fd))) break;
			++dtype;
			ZERNRD(fileno);
			FINDFLDSETSHORT(fileno,"DEFINITION TYPE",dtype);
			FINDFLDSETSTRING(fileno,"ACCOUNT CODE",fd);
			ef=GTENRD(fileno,1);
		}
		DefType=dtype;
		CLSNRD(fileno);
	}
	return(DefType);
}
Wt::WAbstractItemModel *MakeModel(Wt::WContainerWidget *parent)
{
	Wt::WAbstractItemModel *model=NULL;
	BalSumScatter *b=NULL;
	BalSumScatterColumn *c=NULL;
	boost::any data;
	int x=0,y=0;

	if(model!=NULL) model->~WAbstractItemModel();
	model = new Wt::WStandardItemModel(0,0,parent);
	for(x=0,b=BalSumData;x<numBalSumData;++x,++b)
	{
		if(x==0)
		{
			model->insertColumns(model->columnCount(),1);
			model->setHeaderData(0,boost::any(Wt::WString("Period")));
			for(y=0,c=b->C;y<b->num;++y,++c)
			{
				model->insertColumns(model->columnCount(),1);
				model->setHeaderData(y+1,boost::any(Wt::WString(c->reference)));
			}
		}
		model->insertRows(model->rowCount(),1);
		model->setData(x,0,boost::any(Wt::WString(b->period)));
		for(y=0,c=b->C;y<b->num;++y,++c)
		{
			data=boost::any((double)c->amt);	
			model->setData(x,y+1,data);
		}
	}
	return(model);
}
static void NewFund2Scatter()
{
	Wt::WString text;
	std::string s1;

	text=FUND_COMBOBOX->currentText();
	ScatterType=SCATTER_TYPE->currentIndex();
	s1=text.toUTF8();
	ActualBalSumScatter->clear();
	cashFINBREF=0;
	expFINBREF=0;revFINBREF=0;
	fbuFINBREF=0;
	BSScatterWidget(DEFINITION_TYPE,s1.c_str());
}
Wt::WContainerWidget *BSScatterWidget(short dtype2use,char *fd2use)
{
	Wt::WContainerWidget *w=NULL,*w1=NULL;
	Wt::WAbstractItemModel *model=NULL;
	Wt::WPanel *TablePanel=NULL;
	Wt::WLength spc;
	Wt::WText *t=NULL;
	char tempx[512],*desc=NULL,*pFUND_CODE=NULL;
	Wt::WString s,*temp_str=NULL;
	int x=0,which=0;
		
	if(fromCategory!=NULL) Rfree(fromCategory);
	if(toCategory!=NULL) Rfree(toCategory);
	if(ActualBalSumScatter==NULL)
	{
		while(TRUE)
		{
			++DEFINITION_TYPE;
			numBalSumData=0;
			BalSumData=NULL;
			cashFINBREF=0;
			expFINBREF=0;revFINBREF=0;
			fbuFINBREF=0;
			ScatterType=0;
			if(!isEMPTY(FUND_CODE)) pFUND_CODE=stralloc(FUND_CODE);
			if(FUND_CODE!=NULL) Rfree(FUND_CODE);
			
			FUND_CODE=DetermineFund(DEFINITION_TYPE,pFUND_CODE);
			if(isEMPTY(FUND_CODE) && DEFINITION_TYPE>0) 
			{
				prterr("ERROR:  Unable to identify FUND_CODE and build BalSumData.");
				return(NULL);
			}
			DEFINITION_TYPE=DetermineDEFTYPE(FUND_CODE);
			GetBalSumData(DEFINITION_TYPE,FUND_CODE);
			if(BalSumData!=NULL) break;
		}
		w=new Wt::WContainerWidget();
		ActualBalSumScatter=(Wt::WContainerWidget *)w;
	} else {
		DEFINITION_TYPE=dtype2use;
		if(FUND_CODE!=NULL) Rfree(FUND_CODE);
		FUND_CODE=stralloc(fd2use);
		GetBalSumData(DEFINITION_TYPE,FUND_CODE);	
		w=(Wt::WContainerWidget *)ActualBalSumScatter;
	}
	if(pFUND_CODE!=NULL) Rfree(pFUND_CODE);
	w1=new Wt::WContainerWidget(w);
	FUND_COMBOBOX=new Wt::WComboBox(w1);
	if(FUNDS==NULL) FUNDS=GETFUNDS(DEFINITION_TYPE,FUND_CODE);
	FUND_COMBOBOX->addStyleClass("OpenRDA ComboBox");
	for(x=0;x<FUNDS->numlibs;++x) 
	{
		if(!RDAstrcmp(FUNDS->libs[x],FUND_CODE)) which=x;
		temp_str=new Wt::WString(FUNDS->libs[x],UTF8);
		FUND_COMBOBOX->addItem(*temp_str);
		temp_str->~WString();
	}
	FUND_COMBOBOX->setCurrentIndex(which);
	FUND_COMBOBOX->clicked().connect(boost::bind(&NewFund2Scatter));
	FUND_COMBOBOX->activated().connect(boost::bind(&NewFund2Scatter));
	
	desc=FundDescription(DEFINITION_TYPE,FUND_CODE);
	memset(tempx,0,512);
	sprintf(tempx,"%s Cash, Unreserved Fund Balance, Expenditures, and Revenue",(desc!=NULL ? desc:""));
	if(desc!=NULL) Rfree(desc);
	t=new Wt::WText(WString(tempx),w1);
	t->addStyleClass("OpenRDA Label");
	SCATTER_TYPES=APPlibNEW();
	addAPPlib(SCATTER_TYPES,"Simple");
	addAPPlib(SCATTER_TYPES,"Complex");
	SCATTER_TYPE=new Wt::WComboBox(w1);
	SCATTER_TYPE->addStyleClass("OpenRDA ComboBox");
	for(x=0;x<SCATTER_TYPES->numlibs;++x)
	{
		temp_str=new Wt::WString(SCATTER_TYPES->libs[x],UTF8);
		SCATTER_TYPE->addItem(*temp_str);
		temp_str->~WString();
	}
	SCATTER_TYPE->setCurrentIndex(ScatterType);
	SCATTER_TYPE->clicked().connect(boost::bind(&NewFund2Scatter));
	SCATTER_TYPE->activated().connect(boost::bind(&NewFund2Scatter));

	if(BalSumData==NULL) return(w);
	model=MakeModel(w);

	if(!model) 
	{
		prterr("Error: No model.");
		return(w);
	}

	Wt::Chart::WCartesianChart *chart = new Wt::Chart::WCartesianChart(Wt::Chart::ChartType::ScatterPlot,w);
	chart->setModel(model);
	chart->setXSeriesColumn(0);
	chart->setLegendEnabled(TRUE);
	chart->setType(Wt::Chart::ChartType::CategoryChart);
  	chart->axis(XAxis).setScale(CategoryScale); // set scale of X axis to DateScale
	
	chart->axis(YAxis).setLabelFormat("");
	chart->setPlotAreaPadding(100,Left);
	chart->setPlotAreaPadding(20,Top | Bottom);
	for(x=1;x<model->columnCount();++x) 
	{
		WDataSeries s(x,Wt::Chart::SeriesType::BarSeries);
		s.setShadow(WShadow(3,3,WColor(0,0,0,127),3));
		chart->addSeries(s);
	}
	chart->resize(900,500); /* Must give explicit size */
	chart->setMargin(10,Top|Bottom);
	chart->setMargin(Wt::WLength::Auto,Left|Right);
	TablePanel=new Wt::WPanel(w);
	TablePanel->setTitle(Wt::WString("G/L Summary Scatter Table"));
	TablePanel->setTitleBar(TRUE);
	TablePanel->setCollapsible(TRUE);
  	TablePanel->resize(880, WLength::Auto);
  	TablePanel->setMargin(WLength::Auto, Left | Right);
  	TablePanel->setMargin(20, Top | Bottom);
	w1=new Wt::WContainerWidget();
	spc=Wt::WLength(0,Wt::WLength::Pixel);
	w1->setPadding(spc,All);
	TablePanel->setCentralWidget(w1);
	TablePanel->setCollapsed(TRUE);
	Wt::WTableView *table = new Wt::WTableView(w1);
	table->setMargin(10,Top | Bottom);
	table->setMargin(Wt::WLength::Auto, Left | Right);
	table->setModel(model);
	table->setSortingEnabled(FALSE);
	table->setColumnResizeEnabled(TRUE);
	table->setSelectionMode(NoSelection);
	table->setAlternatingRowColors(TRUE);
	table->setRowHeaderCount(1);
	table->setColumnAlignment(0,AlignCenter);
	table->setHeaderAlignment(0,AlignCenter);
	table->setRowHeight(22);
	for(x=1;x<model->columnCount();++x)
	{
		table->setColumnAlignment(x,AlignRight);
		table->setHeaderAlignment(x,AlignRight);
	}

	if(WApplication::instance()->environment().ajax()) 
	{
		table->resize(800,20+5*22);
		table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
	} else {
		table->resize(800,20+5*22+25);
		table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
	}

	Wt::WItemDelegate *delegate = new Wt::WItemDelegate(w);
	delegate->setTextFormat("%.02f");
	table->setItemDelegate(delegate);
	table->setItemDelegateForColumn(0,new Wt::WItemDelegate(w));

	for(x=0;x<model->columnCount();++x)
	{
		table->setColumnWidth(x,120);
	}
	new ChartConfig(chart,w);
	if(SCATTER_TYPES!=NULL) freeapplib(SCATTER_TYPES);
	if(FINBREFS!=NULL) freeapplib(FINBREFS);
	return(w);
}
Wt::WContainerWidget *BalSumScatterWidget()
{
	char *libx=NULL,display_plot=FALSE;
	RDAGenericSetup *gsv=NULL;

	libx=Rmalloc(RDAstrlen(CURRENTDIRECTORY)+17);
#ifndef WIN32
	sprintf(libx,"%s/rda/FINMGT.GSV",CURRENTDIRECTORY);
#endif
#ifdef WIN32
	sprintf(libx,"%s\\rda\\FINMGT.GSV",CURRENTDIRECTORY);
#endif

	gsv=RDAGenericSetupNew("FINMGT","DISPLAY G/L CATEGORY PLOT");
	if(gsv!=NULL)
	{
		if(!getRDAGenericSetupbin(libx,gsv))
		{
			display_plot=*gsv->value.string_value;
		}
	}
	if(gsv!=NULL) FreeRDAGenericSetup(gsv);
	if(libx!=NULL) Rfree(libx);
	if(display_plot==FALSE) return(NULL);
	ActualBalSumScatter=NULL;
	DEFINITION_TYPE=0;
	FUND_CODE=NULL;
	return(BSScatterWidget(0,NULL));
}
