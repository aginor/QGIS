class QgsVectorLayerTools : QObject
{
%TypeHeaderCode
#include <qgsvectorlayertools.h>
%End
  public:
    QgsVectorLayerTools();

    virtual ~QgsVectorLayerTools();

    /**
     * This method should/will be called, whenever a new feature will be added to the layer
     *
     * @param layer           The layer to which the feature should be added
     * @param defaultValues   Default values for the feature to add
     * @param defaultGeometry A default geometry to add to the feature
     * @param feature         Updated feature after adding will be written back to this
     * @return                True in case of success, False if the operation failed/was aborted
     */
    virtual bool addFeature( QgsVectorLayer* layer, const QgsAttributeMap& defaultValues = QgsAttributeMap(), const QgsGeometry& defaultGeometry = QgsGeometry(), QgsFeature* feature /Out/ = 0 ) const = 0;

    /**
     * This will be called, whenever a vector layer should be switched to edit mode. Check the providers
     * capability to edit in here.
     * If successful layer->startEditing() will be called and true returned.
     *
     * @param layer  The layer on which to start an edit session
     *
     * @return       True, if the editing session was started
     */
    virtual bool startEditing( QgsVectorLayer* layer ) const = 0;

    /**
     * Will be called, when an editing session is ended and the features should be commited.
     * Appropriate dialogs should be shown like
     *
     * @param layer       The layer to commit
     * @param allowCancel True if a cancel button should be offered
     * @return            True if successful
     */
    virtual bool stopEditing( QgsVectorLayer* layer, bool allowCancel = true ) const = 0;

    /**
     * Should be called, when the features should be commited but the editing session is not ended.
     *
     * @param layer       The layer to commit
     * @return            True if successful
     */
    virtual bool saveEdits( QgsVectorLayer* layer ) const = 0;

    /**
     * Copy and move features with defined translation.
     *
     * @param layer The layer
     * @param request The request for the features to be moved. It will be assigned to a new feature request with the newly copied features.
     * @param dx The translation on x
     * @param dy The translation on y
     * @param errorMsg If given, it will contain the error message
     * @return True if all features could be copied.
     *
     * TODO QGIS 3: remove const qualifier
     */
    virtual bool copyMoveFeatures( QgsVectorLayer* layer, QgsFeatureRequest &request /In,Out/, double dx = 0, double dy = 0, QString* errorMsg /Out/ = nullptr ) const;
};
