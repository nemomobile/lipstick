#include <QTest>
#include <qdbusextratypes.h>

// A fake implementation of QDBusArgument class for unit-testing object
// serialization to and from QDBusArgument. For all intents and purposes
// this fake behaves the same way as the real QDBusArgument class also
// mimicking the fact that you cannot marshall to a partially
// demashalled QDBusArgument. This should also help catch cases when a
// 'operator >>' function is written incorrectly in respect of object
// constness.
//
// The only real difference is that this QDBusArgument is not in any way
// connected to the real QDBus susbsystem _and_ a marshalled QDBusArgument
// can be unmarshalled as well, so that the following becomes possible in
// unit tests:
//
// MyClass a, b;
//
// // Make a test object 'a'
// ...
//
// // Marshall 'a' to a QDBusArgument and unmarshall into 'b'
// QDBusArgument arg;
// arg << a;
// arg >> b;
//
// // Thest that 'b' is the same as 'a'
// ...

class QDBusArgument
{
    public:
        QDBusArgument();

        // marshalling

        void beginStructure();
        void endStructure();
        void beginArray(int elementMetaTypeId);
        void endArray();
        void beginMap(int keyMetaTypeId, int valueMetaTypeId);
        void endMap();
        void beginMapEntry();
        void endMapEntry();

        QDBusArgument &operator<<(uchar arg);
        QDBusArgument &operator<<(bool arg);
        QDBusArgument &operator<<(short arg);
        QDBusArgument &operator<<(ushort arg);
        QDBusArgument &operator<<(int arg);
        QDBusArgument &operator<<(uint arg);
        QDBusArgument &operator<<(qlonglong arg);
        QDBusArgument &operator<<(qulonglong arg);
        QDBusArgument &operator<<(double arg);
        QDBusArgument &operator<<(const QString &arg);
        QDBusArgument &operator<<(const QDBusVariant &arg);
        QDBusArgument &operator<<(const QDBusObjectPath &arg);
        QDBusArgument &operator<<(const QDBusSignature &arg);
        QDBusArgument &operator<<(const QStringList &arg);
        QDBusArgument &operator<<(const QByteArray &arg);

        // unmarshalling

        void beginStructure() const;
        void endStructure() const;
        void beginArray() const;
        void endArray() const;
        void beginMap() const;
        void endMap() const;
        void beginMapEntry() const;
        void endMapEntry() const;
        bool atEnd() const;

        const QDBusArgument &operator>>(uchar &arg) const;
        const QDBusArgument &operator>>(bool &arg) const;
        const QDBusArgument &operator>>(short &arg) const;
        const QDBusArgument &operator>>(ushort &arg) const;
        const QDBusArgument &operator>>(int &arg) const;
        const QDBusArgument &operator>>(uint &arg) const;
        const QDBusArgument &operator>>(qlonglong &arg) const;
        const QDBusArgument &operator>>(qulonglong &arg) const;
        const QDBusArgument &operator>>(double &arg) const;
        const QDBusArgument &operator>>(QString &arg) const;
        const QDBusArgument &operator>>(QDBusVariant &arg) const;
        const QDBusArgument &operator>>(QDBusObjectPath &arg) const;
        const QDBusArgument &operator>>(QDBusSignature &arg) const;
        const QDBusArgument &operator>>(QStringList &arg) const;
        const QDBusArgument &operator>>(QByteArray &arg) const;

    private:
        void assertMarker(QVariant& arg) const;
        void assertMarshalling() const;
        void assertUnmarshalling() const;

        mutable int index;
        QList<QVariant> data;
};

class QDBusArgumentMarker
{
    public:
        enum Type {
            NONE,
            BEGIN_MAP,
            END_MAP,
            BEGIN_MAP_ENTRY,
            END_MAP_ENTRY,
            BEGIN_STRUCTURE,
            END_STRUCTURE,
            BEGIN_ARRAY,
            END_ARRAY
        };

        QDBusArgumentMarker() : type(NONE) {};
        QDBusArgumentMarker(Type t) : type(t) {};

        bool operator==(const QDBusArgumentMarker& other) const {
            return type == other.type;
        };

        bool operator!=(const QDBusArgumentMarker& other) const {
            return !(*this == other);
        };

        operator QVariant()
        {
            return QVariant::fromValue(*this);
        }

        operator QString()
        {
            switch(type) {
                case NONE: return QString("NONE");
                case BEGIN_MAP: return QString("BEGIN_MAP");
                case END_MAP: return QString("END_MAP");
                case BEGIN_MAP_ENTRY: return QString("BEGIN_MAP_ENTRY");
                case END_MAP_ENTRY: return QString("END_MAP_ENTRY");
                case BEGIN_STRUCTURE: return QString("BEGIN_STRUCTURE");
                case END_STRUCTURE: return QString("END_STRUCTURE");
                case BEGIN_ARRAY: return QString("BEGIN_ARRAY");
                case END_ARRAY: return QString("END_ARRAY");
            }

            return QString();
        }

        Type type;
};

Q_DECLARE_METATYPE(QDBusArgumentMarker);

QDBusArgument::QDBusArgument()
    : index(0)
{
}

// marshalling

void QDBusArgument::beginStructure()
{
    assertMarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::BEGIN_STRUCTURE);
    data.append(token);
}

void QDBusArgument::endStructure()
{
    assertMarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::END_STRUCTURE);
    data.append(token);
}

void QDBusArgument::beginArray(int elementMetaTypeId) 
{
    Q_UNUSED(elementMetaTypeId);

    assertMarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::BEGIN_ARRAY);
    data.append(token);
}

void QDBusArgument::endArray()
{
    assertMarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::END_ARRAY);
    data.append(token);
}

void QDBusArgument::beginMap(int keyMetaTypeId, int valueMetaTypeId)
{
    Q_UNUSED(keyMetaTypeId);
    Q_UNUSED(valueMetaTypeId);

    assertMarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::BEGIN_MAP);
    data.append(token);
}

void QDBusArgument::endMap()
{
    assertMarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::END_MAP);
    data.append(token);
}

void QDBusArgument::beginMapEntry()
{
    assertMarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::BEGIN_MAP_ENTRY);
    data.append(token);
}

void QDBusArgument::endMapEntry()
{
    assertMarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::END_MAP_ENTRY);
    data.append(token);
}

QDBusArgument &QDBusArgument::operator<<(uchar arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(bool arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(short arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(ushort arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(int arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(uint arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(qlonglong arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(qulonglong arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(double arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(const QString &arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(const QDBusVariant &arg)
{
    assertMarshalling();
    data.append(QVariant::fromValue(arg));
    return *this;
}
QDBusArgument &QDBusArgument::operator<<(const QDBusObjectPath &arg)
{
    assertMarshalling();
    data.append(QVariant::fromValue(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(const QDBusSignature &arg)
{
    assertMarshalling();
    data.append(QVariant::fromValue(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(const QStringList &arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

QDBusArgument &QDBusArgument::operator<<(const QByteArray &arg)
{
    assertMarshalling();
    data.append(QVariant(arg));
    return *this;
}

// unmarshaling

void QDBusArgument::assertMarker(QVariant& other) const
{
    QDBusArgumentMarker mine = data.value(index).value<QDBusArgumentMarker>();
    QDBusArgumentMarker his = other.value<QDBusArgumentMarker>();

    if(mine != his) {
        QFAIL(QString("Position %1: %2 marker found, %3 marker expected").
              arg(QString::number(index)).
              arg(mine).
              arg(his).
              toAscii().data());
    }
}

void QDBusArgument::assertMarshalling() const
{
    if (index != 0)
    {
        QFAIL("Cannot marshall a (partially) unmarshalled QDBusArgument");
    }
}

void QDBusArgument::assertUnmarshalling() const
{
    // Can't really make sure we're unmarshalling -- we can always start
    // unmarshalling a partially marshalled QDBusArgument...
}


void QDBusArgument::beginStructure() const
{
    assertUnmarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::BEGIN_STRUCTURE);
    assertMarker(token);
    index++;
}

void QDBusArgument::endStructure() const
{
    assertUnmarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::END_STRUCTURE);
    assertMarker(token);
    index++;
}

void QDBusArgument::beginArray() const
{
    assertUnmarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::BEGIN_ARRAY);
    assertMarker(token);
    index++;
}

void QDBusArgument::endArray() const
{
    assertUnmarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::END_ARRAY);
    assertMarker(token);
    index++;
}

void QDBusArgument::beginMap() const
{
    assertUnmarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::BEGIN_MAP);
    assertMarker(token);
    index++;
}

void QDBusArgument::endMap() const
{
    assertUnmarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::END_MAP);
    assertMarker(token);
    index++;
}

void QDBusArgument::beginMapEntry() const
{
    assertUnmarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::BEGIN_MAP_ENTRY);
    assertMarker(token);
    index++;
}

void QDBusArgument::endMapEntry() const
{
    assertUnmarshalling();
    QVariant token = QDBusArgumentMarker(QDBusArgumentMarker::END_MAP_ENTRY);
    assertMarker(token);
    index++;
}

bool QDBusArgument::atEnd() const
{
    // If we're at the end of the data list, we're "at end"
    if(index >= data.count())
        return true;

    // If the last check didn't get it, value() will return an default
    // (invalid) QVariant, we're at the end
    QVariant v = data.value(index);
    if(!v.isValid())
        return true;

    // If the QVariant is valid and it is not a marker, we're not at the end
    if(!v.canConvert<QDBusArgumentMarker>())
        return false;

    // If this is and end marker, we're at the [logical] end
    QDBusArgumentMarker marker = v.value<QDBusArgumentMarker>();
    if(marker.type == QDBusArgumentMarker::END_ARRAY ||
       marker.type == QDBusArgumentMarker::END_MAP ||
       marker.type == QDBusArgumentMarker::END_STRUCTURE)
        return true;

    // It is a valid marker, but not an end marker, we're not at the end
    return false;
}

const QDBusArgument &QDBusArgument::operator>>(uchar &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<uchar>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(bool &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<bool>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(short &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<short>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(ushort &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<ushort>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(int &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<int>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(uint &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<uint>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(qlonglong &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<qlonglong>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(qulonglong &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<qulonglong>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(double &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<double>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(QString &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<QString>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(QDBusVariant &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<QDBusVariant>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(QDBusObjectPath &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<QDBusObjectPath>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(QDBusSignature &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<QDBusSignature>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(QStringList &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<QStringList>();
    return *this;
}

const QDBusArgument &QDBusArgument::operator>>(QByteArray &arg) const
{
    assertUnmarshalling();
    arg = data.value(index++).value<QByteArray>();
    return *this;
}

const QDBusArgument &operator>>(const QDBusArgument &a, QVariant &v)
{
    QDBusVariant dbv;
    a >> dbv;
    v = dbv.variant();
    return a;
}
