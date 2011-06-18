#ifndef TEECHARTDEFINES_H
#define TEECHARTDEFINES_H

//EConstants
const unsigned long clTeeColor = 536870912;
const unsigned long clNone = 536870911;

// EAxisLabelStyle
const unsigned long talAuto = 0;
const unsigned long talNone = 1;
const unsigned long talValue = 2;
const unsigned long talMark = 3;
const unsigned long talText = 4;

// EAxisType
const unsigned long atLeft = 0;
const unsigned long atTop = 1;
const unsigned long atRight = 2;
const unsigned long atBottom = 3;
const unsigned long atDepth = 4;

// EBarStyles
const unsigned long bsRectangle = 0;
const unsigned long bsPyramid = 1;
const unsigned long bsInvPyramid = 2;
const unsigned long bsCilinder = 3;
const unsigned long bsEllipse = 4;
const unsigned long bsArrow = 5;
const unsigned long bsRectGradient = 6;

// EBorderStyle
const unsigned long bsNone = 0;
const unsigned long bsSingle = 1;

// EBrushStyle
const unsigned long bsSolid = 0;
const unsigned long bsClear = 1;
const unsigned long bsHorizontal = 2;
const unsigned long bsVertical = 3;
const unsigned long bsFDiagonal = 4;
const unsigned long bsBDiagonal = 5;
const unsigned long bsCross = 6;
const unsigned long bsDiagCross = 7;

// ECandleStyle 
const unsigned long csCandleBar = 0;
const unsigned long csCandleStick = 1;

// EChartPenStyle
const unsigned long psSolid = 0;
const unsigned long psDash = 1;
const unsigned long psDot = 2;
const unsigned long psDashDot = 3;
const unsigned long psDashDotDot = 4;
const unsigned long psClear = 5;
const unsigned long psInsideFrame = 6;

// EChartScroll
const unsigned long pmNone = 0;	
const unsigned long pmHorizontal = 1;
const unsigned long pmVertical = 2;
const unsigned long pmBoth = 3;

// EDateTimeStep
const unsigned long dtOneSecond = 0;
const unsigned long dtFiveSeconds = 1;
const unsigned long dtTenSeconds = 2;
const unsigned long dtFifteenSeconds = 3;
const unsigned long dtThirtySeconds = 4;
const unsigned long dtOneMinute = 5;
const unsigned long dtFiveMinutes = 6;
const unsigned long dtTenMinutes = 7;
const unsigned long dtFifteenMinutes = 8;
const unsigned long dtThirtyMinutes = 9;
const unsigned long dtOneHour = 10;
const unsigned long dtTwoHours = 11;
const unsigned long dtSixHours = 12;
const unsigned long dtTwelveHours = 13;
const unsigned long dtOneDay = 14;
const unsigned long dtTwoDays = 15;
const unsigned long dtThreeDays = 16;
const unsigned long dtOneWeek = 17;
const unsigned long dtHalfMonth = 18;
const unsigned long dtOneMonth = 19;
const unsigned long dtTwoMonths = 20;
const unsigned long dtSixMonths = 21;
const unsigned long dtOneYear = 22;

// EDragMode
const unsigned long dmManual = 0;;
const unsigned long dmAutomatic = 1;;

// EDragState 
const unsigned longdsDragEnter = 0;
const unsigned longdsDragLeave = 1;
const unsigned longdsDragMove = 2;

// EErrorWidthUnits 
const unsigned ewuPercent = 0;
const unsigned ewuPixels= 1;

// EFunctionType
const unsigned long tfCopy = 0;
const unsigned long tfAdd = 1;
const unsigned long tfSubtract = 2;
const unsigned long tfMultiply = 3;
const unsigned long tfDivide = 4;
const unsigned long tfAverage = 5;
const unsigned long tfHigh = 6;
const unsigned long tfLow = 7;
const unsigned long tfMovavg = 8;
const unsigned long tfExpavg = 9;
const unsigned long tfMomentum = 10;
const unsigned long tfCurveFit = 11;
const unsigned long tfRSI = 12;
const unsigned long tfTrend = 13;
const unsigned long tfStdDeviation = 14;
const unsigned long tfCumulative = 15;
const unsigned long tfCount = 16;

// EGradientDirection 
const unsigned long gdTopBottom = 0;
const unsigned long gdBottomTop = 1;
const unsigned long gdLeftRight = 2;
const unsigned long gdRightLeft = 3;
const unsigned long gdFromCenter = 4;
const unsigned long gdFromTopLeft = 5;
const unsigned long gdFromBottomLeft = 6;


// EHorizontalAxis
const unsigned long aTopAxis = 0;
const unsigned long aBottomAxis = 1;
const unsigned long aBothHorizAxis = 2;
const unsigned long aCustomHorizAxis = 3;

// EJPEGPerformance
const unsigned long jpegBestQuality = 0;
const unsigned long jpegBestSpeed = 1;

// ELegendAlignment
const unsigned long laLeft = 0;
const unsigned long laRight = 1;
const unsigned long laTop = 2;
const unsigned long laBottom = 3;

// ELegendStyle
const unsigned long lsAuto = 0;
const unsigned long lsSeries = 1;
const unsigned long lsValues = 2;
const unsigned long lsLastValues = 3;

// ELegendTextStyle
const unsigned long ltsPlain = 0;
const unsigned long ltsLeftValue =1;
const unsigned long ltsRightValue =2;
const unsigned long ltsLeftPercent = 3;
const unsigned long ltsRightPercent = 4;
const unsigned long ltsXValue = 5;

// EMarkStyle
const unsigned long smsValue = 0;
const unsigned long smsPercent = 1;
const unsigned long smsLabel = 2;
const unsigned long smsLabelPercent = 3;
const unsigned long smsLabelValue = 4;
const unsigned long smsLegend = 5;
const unsigned long smsPercentTotal = 6;
const unsigned long smsLabelPercentTotal = 7;
const unsigned long smsXValue = 8;

// EMouseButton
const unsigned long mbLeft = 1;
const unsigned long mbRight	= 2;
const unsigned long mbMiddle = 4;

// EMultiArea
const unsigned long maNone = 0;
const unsigned long maStacked = 1;
const unsigned long maStacked100 = 2;

// EMultiBar
const unsigned long mbNone = 0;
const unsigned long mbSide = 1;
const unsigned long mbStacked = 2;
const unsigned long mbStacked100 = 3;

// EPanelBevel
const unsigned long bvNone = 0;
const unsigned long bvLowered = 1;
const unsigned long bvRaised = 2;

// EPointerStyle
const unsigned long psRectangle = 0;
const unsigned long psCircle = 1;
const unsigned long psTriangle = 2;
const unsigned long psDownTriangle = 3;
const unsigned long psCross = 4;
const unsigned long psDiagCross = 5;
const unsigned long psStar = 6;
const unsigned long psDiamond = 7;
const unsigned long psSmallDot = 8;

// EPrinterOrientation
const unsigned long poPortrait = 0;
const unsigned long poLandscape = 1;

// ESeriesClass
const unsigned long scLine = 0;
const unsigned long scBar = 1;
const unsigned long scHorizBar = 2;
const unsigned long scArea = 3;
const unsigned long scPoint = 4;
const unsigned long scPie = 5;
const unsigned long scFastLine = 6;
const unsigned long scShape = 7;
const unsigned long scGantt = 8;
const unsigned long scBubble = 9;
const unsigned long scArrow = 10;
const unsigned long scCandle = 11;
const unsigned long scPolar = 12;
const unsigned long scSurface = 13;
const unsigned long scVolume = 14;
const unsigned long scErrorBar = 15;
const unsigned long scBezier = 16;
const unsigned long scContour = 17;
const unsigned long scError = 18;
const unsigned long scPoint3D = 19;
const unsigned long scRadar = 20;

// EShapeStyle
const unsigned long chasRectangle = 0;
const unsigned long chasCircle = 1;
const unsigned long chasVertLine = 2	;
const unsigned long chasHorizLine = 3	;
const unsigned long chasTriangle = 4;
const unsigned long chasInvertTriangle = 5;
const unsigned long chasLine = 6;
const unsigned long chasDiamond = 7;
const unsigned long chasCube = 8;
const unsigned long chasCross = 9;
const unsigned long chasDiagCross = 10;
const unsigned long chasStar = 11;
const unsigned long chasPyramid = 12;
const unsigned long chasInvertPyramid = 13;

// EShapeXYStyle
const unsigned long xysPixels = 0;
const unsigned long xysAxis = 1;
const unsigned long xysAxisOrigin = 2;

// EShiftState
const unsigned long ssShift = 1;
const unsigned long ssAlt = 2;
const unsigned long ssCtrl = 4;
const unsigned long ssLeft = 8;
const unsigned long ssRight = 16;
const unsigned long ssMiddle = 5;
const unsigned long ssDouble = 6;

// ETeeBackImageMode 
const unsigned long pbmStretch = 0;
const unsigned long pbmTile = 1;
const unsigned long pbmCenter = 2;

// ETextAlignment
const unsigned long aLeft = 0;
const unsigned long aRight = 1;
const unsigned long aCenter = 2;

// ETitleAlignment
const unsigned long taLeftJustify = 0;
const unsigned long taRightJustify = 1;
const unsigned long taCenter = 2;

// ETypeFitting
const unsigned long cfPolynomial = 0;
const unsigned long cfLogarithmic = 1;
const unsigned long cfExponential = 2;

//EValueListOrder
const unsigned long loNone = 0;
const unsigned long loAscending = 1;
const unsigned long loDescending = 2;

//EVerticalAxis
const unsigned long aLeftAxis = 0;
const unsigned long aRightAxis = 1;
const unsigned long aBothVertAxis = 2;
const unsigned long aCustomVertAxis = 3;

//EErrorSeriesStyle
const unsigned long essLeft = 0;
const unsigned long essRight = 1;
const unsigned long essLeftRight = 2;
const unsigned long essTop = 3;
const unsigned long essBottom = 4;
const unsigned long essTopBottom = 5;

//EFunctionPeriodAlign
const unsigned long paFirst = 0;
const unsigned long paCenter = 1;
const unsigned long paLast = 2;

//EFunctionPeriodStyle
const unsigned long psNumPoints = 0;
const unsigned long psRange = 1;

//ETeeEditorPage
const unsigned long epMain = 0;
const unsigned long epGeneral = 1;
const unsigned long epAxis = 2;
const unsigned long epTitles = 3;
const unsigned long epLegend = 4;
const unsigned long epPanel = 5;
const unsigned long epPaging = 6;
const unsigned long epWalls = 7;
const unsigned long epAspect = 8;
const unsigned long epSeriesGeneral = 9;
const unsigned long epSeriesMarks = 10;

#endif
