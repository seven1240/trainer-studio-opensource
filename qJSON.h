#ifndef QJSON_H
#define QJSON_H

#include <qvariant.h>
#include <qbytearray.h>
#include <cjson.h>

class qJSON
{
public:
    qJSON();
    ~qJSON();
    void parse(char *s, bool *ok);
    QVariantMap toMap();

private:
    cJSON *_json;

    QVariantMap toMap(cJSON *json_object, int max_depth);
};

#endif // QJSON_H
