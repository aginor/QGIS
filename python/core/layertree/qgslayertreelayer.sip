/**
 * Layer tree node points to a map layer.
 *
 * When using with existing QgsMapLayer instance, it is expected that the layer
 * has been registered in QgsMapLayerRegistry earlier.
 *
 * The node can exist also without a valid instance of a layer (just ID). That
 * means the referenced layer does not need to be loaded in order to use it
 * in layer tree. In such case, the node will start listening to map layer
 * registry updates in expectation that the layer (identified by its ID) will
 * be loaded later.
 *
 * A map layer is supposed to be present in one layer tree just once. It is
 * however possible that temporarily a layer exists in one tree more than just
 * once, e.g. while reordering items with drag and drop.
 *
 * @note added in 2.4
 */
class QgsLayerTreeLayer : QgsLayerTreeNode
{
%TypeHeaderCode
#include <qgslayertreelayer.h>
%End

  public:
    explicit QgsLayerTreeLayer( QgsMapLayer* layer );

    explicit QgsLayerTreeLayer( const QString& layerId, const QString& name = QString() );

    QString layerId() const;

    QgsMapLayer* layer() const;

    //! Get layer's name
    //! @note added in 3.0
    QString name() const;
    //! Set layer's name
    //! @note added in 3.0
    void setName( const QString& n );

    Qt::CheckState isVisible() const;
    void setVisible( Qt::CheckState visible );

    static QgsLayerTreeLayer* readXml( QDomElement& element ) /Factory/;
    virtual void writeXml( QDomElement& parentElement );

    virtual QString dump() const;

    virtual QgsLayerTreeLayer* clone() const /Factory/;

  protected slots:
    void registryLayersAdded( const QList<QgsMapLayer*>& layers );
    void registryLayersWillBeRemoved( const QStringList& layerIds );
    //! Emits a nameChanged() signal if layer's name has changed
    //! @note added in 3.0
    void layerNameChanged();

  signals:
    //! emitted when a previously unavailable layer got loaded
    void layerLoaded();
    //! emitted when a previously available layer got unloaded (from layer registry)
    //! @note added in 2.6
    void layerWillBeUnloaded();

  protected:
    void attachToLayer();

  private:
    QgsLayerTreeLayer( const QgsLayerTreeLayer& other );

};
