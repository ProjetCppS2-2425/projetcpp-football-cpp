// QrCode.hpp (stub - à remplacer par le vrai code de la lib Nayuki)
#pragma once
#include <string>
#include <vector>

namespace qrcodegen {
class QrCode {
public:
    enum class Ecc { LOW, MEDIUM, QUARTILE, HIGH };
    static QrCode encodeText(const char *text, Ecc ecl);
    int getSize() const;
    bool getModule(int x, int y) const;
};
}
