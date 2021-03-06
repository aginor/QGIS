/***************************************************************************
    qgsdatadefinedbutton.h - Data defined selector button
     --------------------------------------
    Date                 : 27-April-2013
    Copyright            : (C) 2013 by Larry Shaffer
    Email                : larrys at dakcarto dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSDATADEFINEDBUTTON_H
#define QGSDATADEFINEDBUTTON_H

#include <QDialog>
#include <QFlags>
#include <QMap>
#include <QPointer>
#include <QToolButton>
#include <QScopedPointer>
#include "qgsexpressioncontextgenerator.h"

class QgsVectorLayer;
class QgsDataDefined;
class QgsMapCanvas;

/** \ingroup gui
 * \class QgsDataDefinedAssistant
 * An assistant (wizard) dialog, accessible from a QgsDataDefinedButton.
 * Can be used to guide users through creation of an expression for the
 * data defined button.
 * @note added in 2.10
 */
class GUI_EXPORT QgsDataDefinedAssistant: public QDialog
{
    Q_OBJECT

  public:
    QgsDataDefinedAssistant() : mMapCanvas( nullptr ) {}

    /**
     * Get the data defined which was defined by this assistant.
     * Needs to be reimplemented by subclasses.
     */
    virtual QgsDataDefined dataDefined() const = 0;

    /** Sets the map canvas associated with the widget. This allows the widget to retrieve the current
     * map scale and other properties from the canvas.
     * @param canvas map canvas
     * @see mapCanvas()
     * @note added in QGIS 2.12
     */
    virtual void setMapCanvas( QgsMapCanvas* canvas ) { mMapCanvas = canvas; }

    /** Returns the map canvas associated with the widget.
     * @see setMapCanvas
     * @note added in QGIS 2.12
     */
    const QgsMapCanvas* mapCanvas() const { return mMapCanvas; }

  protected:

    QgsMapCanvas* mMapCanvas;
};

/** \ingroup gui
 * \class QgsDataDefinedButton
 * A button for defining data source field mappings or expressions.
 */

class GUI_EXPORT QgsDataDefinedButton: public QToolButton
{
    Q_OBJECT
    Q_PROPERTY( QString usageInfo READ usageInfo WRITE setUsageInfo )

  public:
    enum DataType
    {
      String  = 1,
      Int     = 2,
      Double  = 4,
      AnyType = String | Int | Double
    };
    Q_DECLARE_FLAGS( DataTypes, DataType )

    /**
     * Construct a new data defined button
     *
     * @param parent The parent QWidget
     * @param vl Pointer to the associated vector layer
     * @param datadefined Data defined property
     * @param datatypes The expected data types to be compared against the variant type of the QgsField from data source and expression result
     * @param description The description of expected input data
     */
    QgsDataDefinedButton( QWidget* parent = nullptr,
                          const QgsVectorLayer* vl = nullptr,
                          const QgsDataDefined* datadefined = nullptr,
                          QgsDataDefinedButton::DataTypes datatypes = AnyType,
                          const QString& description = QString() );
    ~QgsDataDefinedButton();

    /**
     * Initialize a newly constructed data defined button (useful if button already included from form layout)
     *
     * @param vl Pointer to the associated vector layer
     * @param datadefined Data defined property
     * @param datatypes The expected data types to be compared against the variant type of the QgsField from data source and expression result
     * @param description The description of expected input data
     */
    void init( const QgsVectorLayer* vl,
               const QgsDataDefined* datadefined = nullptr,
               QgsDataDefinedButton::DataTypes datatypes = AnyType,
               const QString& description = QString() );

    QMap< QString, QString > definedProperty() const { return mProperty; }

    /** Updates a QgsDataDefined with the current settings from the button
     * @param dd QgsDataDefined to update
     * @note added in QGIS 2.9
     * @see currentDataDefined
     */
    void updateDataDefined( QgsDataDefined* dd ) const;

    /** Returns a QgsDataDefined which reflects the current settings from the
     * button.
     * @note added in QGIS 2.9
     * @see updateDataDefined
     */
    QgsDataDefined currentDataDefined() const;

    /** Sets the vector layer associated with the button. This controls which fields are
     * displayed within the widget's pop up menu.
     * @param layer vector layer
     * @note added in QGIS 3.0
     */
    void setVectorLayer( QgsVectorLayer* layer );

    /**
     * Whether the current data definition or expression is to be used
     */
    bool isActive() const { return mProperty.value( QStringLiteral( "active" ) ).toInt(); }

    /**
     * Whether the current expression is to be used instead of field mapping
     */
    bool useExpression() const { return mProperty.value( QStringLiteral( "useexpr" ) ).toInt(); }

    /**
     * The current defined expression
     */
    QString getExpression() const { return mProperty.value( QStringLiteral( "expression" ) ); }

    /**
     * The current defined field
     */
    QString getField() const { return mProperty.value( QStringLiteral( "field" ) ); }

    /**
     * The current definition
     * @returns empty QString if not active, otherwise currently defined expression or field name
     */
    QString currentDefinition() const { return mCurrentDefinition; }

    /**
     * The valid data types that will work for the definition (QVariant-coercible to expected type)
     * Compared against the variant type of the QgsField from data source and expression result
     */
    const DataTypes& validDataTypes() const { return mDataTypes; }

    /**
     * The full definition description and current definition (internally generated on a contextual basis)
     */
    QString fullDescription() const { return mFullDescription; }

    /**
     * The usage information about this data definition
     */
    QString usageInfo() const { return mUsageInfo; }

    /**
     * Set the usage information about this data definition
     */
    void setUsageInfo( const QString& info ) { mUsageInfo = info; updateGui(); }

    /**
     * Register list of sibling widgets that get disabled/enabled when data definition or expression is set/unset
     */
    void registerEnabledWidgets( const QList<QWidget*>& wdgts );

    /**
     * Register a sibling widget that gets disabled/enabled when data definition or expression is set/unset
     */
    void registerEnabledWidget( QWidget* wdgt );

    /**
     * Return widget siblings that get disabled/enabled when data definition or expression is set/unset
     *
     * @return unguarded pointers from guarded ones
     */
    QList<QWidget*> registeredEnabledWidgets();

    /**
     * Clears list of sibling widgets
     */
    void clearEnabledWidgets() { mEnabledWidgets.clear(); }

    /**
     * Register list of sibling widgets that get checked when data definition or expression is active
     */
    void registerCheckedWidgets( const QList<QWidget*>& wdgts );

    /**
     * Register a sibling widget that get checked when data definition or expression is active
     */
    void registerCheckedWidget( QWidget* wdgt );

    /**
     * Return widget siblings that get checked when data definition or expression is active
     *
     * @return unguarded pointers from guarded ones
     */
    QList<QWidget*> registeredCheckedWidgets();

    /**
     * Clears list of checkable sibling widgets
     */
    void clearCheckedWidgets() { mCheckedWidgets.clear(); }

    /**
     * Register an expression context generator class that will be used to retrieve
     * an expression context for the button.
     * @param generator A QgsExpressionContextGenerator class that will be used to
     *                  create an expression context when required.
     * @note added in QGIS 3.0
     */
    void registerExpressionContextGenerator( QgsExpressionContextGenerator* generator );

    /**
     * Sets an assistant used to define the data defined object properties.
     * Ownership of the assistant is transferred to the widget.
     * @param title menu title for the assistant
     * @param assistant data defined assistant. Set to null to remove the assistant
     * option from the button.
     * @note added in 2.10
     * @see assistant()
     */
    void setAssistant( const QString& title, QgsDataDefinedAssistant * assistant );

    /** Returns the assistant used to defined the data defined object properties, if set.
     * @see setAssistant()
     * @note added in QGIS 2.12
     */
    QgsDataDefinedAssistant* assistant();

    /**
     * Common descriptions for expected input values
     */
    static QString trString();
    static QString charDesc();
    static QString boolDesc();
    static QString anyStringDesc();
    static QString intDesc();
    static QString intPosDesc();
    static QString intPosOneDesc();
    static QString doubleDesc();
    static QString doublePosDesc();
    static QString double0to1Desc();
    static QString doubleXYDesc();
    static QString double180RotDesc();
    static QString intTranspDesc();
    static QString unitsMmMuDesc();
    static QString unitsMmMuPercentDesc();
    static QString colorNoAlphaDesc();
    static QString colorAlphaDesc();
    static QString textHorzAlignDesc();
    static QString textVertAlignDesc();
    static QString penJoinStyleDesc();
    static QString blendModesDesc();
    static QString svgPathDesc();
    static QString filePathDesc();
    static QString paperSizeDesc();
    static QString paperOrientationDesc();
    static QString horizontalAnchorDesc();
    static QString verticalAnchorDesc();
    static QString gradientTypeDesc();
    static QString gradientCoordModeDesc();
    static QString gradientSpreadDesc();
    static QString lineStyleDesc();
    static QString capStyleDesc();
    static QString fillStyleDesc();
    static QString markerStyleDesc();
    static QString customDashDesc();

  public slots:

    /**
     * Set whether the current data definition or expression is to be used
     */
    void setActive( bool active );

    /**
     * Set siblings' enabled property when data definition or expression is set/unset
     */
    void disableEnabledWidgets( bool disable );

    /**
     * Set siblings' checked property when data definition or expression is active
     */
    void checkCheckedWidgets( bool check );

  signals:

    /**
     * Emitted when data definition or expression is changed
     * @param definition The current definition or expression (empty string if inactive)
     */
    void dataDefinedChanged( const QString& definition );

    /**
     * Emitted when active state changed
     * @param active Whether the definition is active
     */
    void dataDefinedActivated( bool active );

  protected:
    void mouseReleaseEvent( QMouseEvent *event ) override;

    /**
     * Set whether the current expression is to be used instead of field mapping
     */
    void setUseExpression( bool use ) { mProperty.insert( QStringLiteral( "useexpr" ), use ? "1" : "0" ); }

    /**
     * Set the current defined expression
     */
    void setExpression( const QString& exp ) { mProperty.insert( QStringLiteral( "expression" ), exp ); }

    /**
     * Set the current defined field
     */
    void setField( const QString& field ) { mProperty.insert( QStringLiteral( "field" ), field ); }

  private:
    void showDescriptionDialog();
    void showExpressionDialog();
    void showAssistant();
    void updateGui();
    void updateFieldLists();

    const QgsVectorLayer* mVectorLayer;
    QStringList mFieldNameList;
    QStringList mFieldTypeList;
    QMap< QString, QString > mProperty;
    QList< QPointer<QWidget> > mEnabledWidgets;
    QList< QPointer<QWidget> > mCheckedWidgets;

    QMenu* mDefineMenu;
    QAction* mActionDataTypes;
    QMenu* mFieldsMenu;
    QMenu* mVariablesMenu;
    QAction* mActionVariables;

    QAction* mActionActive;
    QAction* mActionDescription;
    QAction* mActionExpDialog;
    QAction* mActionExpression;
    QAction* mActionPasteExpr;
    QAction* mActionCopyExpr;
    QAction* mActionClearExpr;
    QAction* mActionAssistant;

    DataTypes mDataTypes;
    QString mDataTypesString;
    QString mInputDescription;
    QString mFullDescription;
    QString mUsageInfo;
    QString mCurrentDefinition;

    QScopedPointer<QgsDataDefinedAssistant> mAssistant;

    QIcon mIconDataDefine;
    QIcon mIconDataDefineOn;
    QIcon mIconDataDefineError;
    QIcon mIconDataDefineExpression;
    QIcon mIconDataDefineExpressionOn;
    QIcon mIconDataDefineExpressionError;

    QgsExpressionContextGenerator* mExpressionContextGenerator;

  private slots:
    void aboutToShowMenu();
    void menuActionTriggered( QAction* action );
};

Q_DECLARE_OPERATORS_FOR_FLAGS( QgsDataDefinedButton::DataTypes )


#endif // QGSDATADEFINEDBUTTON_H
