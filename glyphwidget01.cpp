#include "glyphwidget.h"

#define TRUNC(x) ((x) >> 6)

GlyphWidget::GlyphWidget(QWidget *parent)
    : QWidget{parent}
    , m_glyphImage(nullptr)
    , m_ftLib(0)
    , m_ftFace(0)
{
    FT_Error ftErr = FT_Err_Ok;
    ftErr = FT_Init_FreeType(&m_ftLib);
}

GlyphWidget::~GlyphWidget()
{
    if (m_ftFace) {
        FT_Done_Face(m_ftFace);
    }

    if (m_ftLib) {
        FT_Done_Library(m_ftLib);
    }

    if (m_glyphImage) {
        delete m_glyphImage;
    }
}


int GlyphWidget::findOptimalGlyphSize ()
{
    if (m_ftFace == 0) {
        return -1;
    }

    int low = 1;
    int high = 1000;
    int bestSize = 0;
    while (low <= high) {
        int mid = (low + high) / 2;
        FT_Set_Pixel_Sizes(m_ftFace, 0, mid);
        FT_Load_Char(m_ftFace, m_glyphKey.character().unicode(), FT_LOAD_DEFAULT);

        // int glyphHeight = (m_ftFace->glyph->metrics.height >> 6);  // Переводим из 26.6 fixed-point в пиксели
        int glyphHeight = m_ftFace->glyph->bitmap.rows;
        // int glyphHeight = (m_ftFace->ascender - m_ftFace->descender) >> 6;
        // int glyphWidth = m_ftFace->glyph->metrics.width >> 6;  // Переводим из 26.6 fixed-point в пиксели

        // if (glyphHeight <= height() * 4 / 5)
        //     return mid;

        if (glyphHeight <= height() * 4 / 5) { // || glyphWidth <= maxWidth) {
            bestSize = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // qDebug()() << "Best Size: " << bestSize << ", low: " << low << ", high: " << high << ", Height: " << height() * 4 / 5;

    return bestSize;
}


void GlyphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (!(m_glyphKey.isValid() && m_glyphEntry.isValid() && m_glyphEntry.glyphPixelsIsValid()))
        return;

    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);

    int startX = (width() - m_glyphKey.gridSize() * m_gridCellSize) / 2;
    int startY = (height() - m_glyphKey.gridSize() * m_gridCellSize) / 2;
    int left = startX + m_glyphEntry.glyphLeft() * m_gridCellSize;
    int top = startY + m_glyphEntry.glyphTop() * m_gridCellSize;

    // Горизонтальные линии
    for (int y = 0; y <= m_glyphKey.gridSize(); ++y) {
        painter.drawLine(
            startX,
            startY + y * m_gridCellSize,
            startX + m_glyphKey.gridSize() * m_gridCellSize,
            startY + y * m_gridCellSize
            );
    }

    // Вертикальные линии
    for (int x = 0; x <= m_glyphKey.gridSize(); ++x) {
        painter.drawLine(
            startX + x * m_gridCellSize,
            startY,
            startX + x * m_gridCellSize,
            startY + m_glyphKey.gridSize() * m_gridCellSize
            );
    }

    if (m_glyphImage != nullptr)
    {
        QImage glyphImage = m_glyphImage->scaled(m_gridCellSize * m_glyphEntry.glyphCols(), m_gridCellSize * m_glyphEntry.glyphRows(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawImage(
            left,
            top,
            glyphImage
            );
    }

    // qDebug()() << "Pixels Vector Size: " << m_glyphPixels.size() << ((m_glyphPixels.size() == m_glyphRows * m_glyphCols) ? "=" : "≠") << m_glyphRows * m_glyphCols;
    if (m_glyphEntry.glyphPixelsIsValid()) {
        for(qint32 y = 0; y < m_glyphEntry.glyphRows(); ++y) {
            for(qint32 x = 0; x < m_glyphEntry.glyphCols(); ++x) {
                // Получаем значение пикселя (true - закрашен, false - пустой)
                // bool pixel = m_glyphPixels[y * m_glyphCols + x];
                bool pixel = m_glyphEntry.getPixel(x, y);

                // Рассчитываем координаты квадрата с учетом масштаба
                QRect pixelRect(
                    left + x * m_gridCellSize,
                    top + y * m_gridCellSize,
                    m_gridCellSize,
                    m_gridCellSize
                    );

                // Отрисовываем квадрат
                if(pixel) {
                    painter.fillRect(pixelRect, QColor(0x33, 0x33, 0xFF , 0xEF));
                } else {
                    painter.fillRect(pixelRect, QColor(0x33, 0, 0, 0x11));
                    painter.setPen(Qt::black);
                    painter.drawRect(pixelRect);
                }
            }
        }
    }

    painter.end();
}

void GlyphWidget::resetGlyphPixels ()
{

}

void GlyphWidget::calcGlyphRect ()
{
    QRect glyphRect(QPoint(m_ftFace->glyph->bitmap_left, -m_ftFace->glyph->bitmap_top), QSize(m_ftFace->glyph->bitmap.width, m_ftFace->glyph->bitmap.rows));
    qDebug() << "Glyph Rect" << glyphRect;
}

void GlyphWidget::renderGlyphPixels ()
{
    FT_Error ftError = FT_Err_Ok;
    resetGlyphPixels();

    if (m_ftFace == 0) {
        qDebug() << "Font Face Not inited";
        return;
    }

    if (!m_glyphKey.isValid() || !m_glyphEntry.isValid())
        return;

    FT_UInt glyph_index = 0;
    ftError = FT_Set_Pixel_Sizes(m_ftFace, 0, m_glyphEntry.glyphSize());
    if (!ftError) {
        // qDebug() << "Set pixel sizes to " << m_glyphSize;
    } else {
        qDebug() << "Can't set pixel sizes to " << m_glyphKey.character();
    }

    glyph_index = FT_Get_Char_Index(m_ftFace, m_glyphKey.character().unicode());

    ftError = FT_Load_Glyph(m_ftFace, glyph_index, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);

    calcGlyphRect ();
    QRect glyphRect(QPoint(m_ftFace->glyph->bitmap_left, m_glyphKey.gridSize() - m_ftFace->glyph->bitmap_top), QSize(m_ftFace->glyph->bitmap.width, m_ftFace->glyph->bitmap.rows));
    m_glyphEntry.setGlyphRect(glyphRect);
    qDebug() << "Glyph Rect: " << glyphRect;

    if (!ftError)
    {
        // setFixedSize(m_glyphRect.width(),
        //              m_glyphRect.height());
        // qDebug() << "Loaded Glyph " << m_character << m_ftFace->glyph->bitmap.width << m_ftFace->glyph->bitmap.rows;

        FT_Bitmap* bitmap = &m_ftFace->glyph->bitmap;
        // qDebug() << "Columns: " << bitmap->width << ", Rows: " << bitmap->rows;
        m_glyphEntry.resetGlyphPixels();
        if (bitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
            for (int y = 0; y < bitmap->rows; y++) {
                for (int x = 0; x < bitmap->width; x++) {
                    // Получаем байт, содержащий пиксель
                    unsigned char byte = bitmap->buffer[y * bitmap->pitch + (x / 8)];
                    // Извлекаем бит (MSB → x=0, LSB → x=7)
                    unsigned char bit = (byte >> (7 - (x % 8))) & 0x1;
                    // printf("%c", bit ? '#' : ' ');
                    // m_glyphPixels.append(bit ? true : false);
                    m_glyphEntry.addPixel(bit ? true : false);
                }
            }
        }
    } else {
        qDebug() << "Error!";
    }
}

void GlyphWidget::renderGlyphImage()
{
    if (m_ftFace == 0) {
        return;
    }

    if (m_glyphImage) {
        delete m_glyphImage;
        m_glyphImage = nullptr;
    }

    if (!m_glyphKey.isValid() || !m_glyphEntry.isValid())
        return;

    // int glyphSize = findOptimalGlyphSize();
    // qDebug()() << "Optimal Font Size: " << glyphSize << m_glyphCols * m_gridCellSize << "x" << m_glyphRows * m_gridCellSize;
    // Устанавливаем размер и загружаем глиф
    int glyphWidth = m_glyphEntry.glyphCols() * m_gridCellSize * 2;
    int glyphHeight = m_glyphEntry.glyphRows() * m_gridCellSize * 2;
    FT_Set_Pixel_Sizes(m_ftFace, glyphWidth, glyphHeight);
#if 1
    FT_Load_Char(m_ftFace, m_glyphKey.character().unicode(), FT_LOAD_RENDER);
#else
    FT_Load_Char(m_ftFace, m_character.unicode(), FT_LOAD_RENDER | FT_LOAD_COLOR);
#endif

    FT_GlyphSlot glyph = m_ftFace->glyph;
    FT_Bitmap bitmap = glyph->bitmap;

    calcGlyphRect ();

    // Создаём QImage с форматом ARGB32 (для прозрачности)
    m_glyphImage = new QImage(bitmap.width, bitmap.rows, QImage::Format_ARGB32);
    m_glyphImage->fill(Qt::transparent);

    // Заполняем пиксели (для монохромного и grayscale-глифа)
    for (int y = 0; y < bitmap.rows; ++y) {
        for (int x = 0; x < bitmap.width; ++x) {
            unsigned char pixel;

            // Обрабатываем монохромный (1 бит на пиксель) и grayscale (8 бит)
            if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
                pixel = (bitmap.buffer[y * bitmap.pitch + (x / 8)] >> (7 - (x % 8))) & 1 ? 0xEF : 0;
            } else { // FT_PIXEL_MODE_GRAY
                pixel = bitmap.buffer[y * bitmap.pitch + x];
            }

            // Устанавливаем пиксель в QImage (чёрный с альфа-каналом)
            m_glyphImage->setPixel(x, y, qRgba(0xFF, 0x33, 0x33, pixel));
        }
    }
}

void GlyphWidget::updateGlyph () {
    qDebug() << __FILE__ << __FUNCTION__;
    if (m_glyphKey.isValid() && m_glyphEntry.isValid()) {
        m_gridCellSize = height() * 4 / (5 * m_glyphKey.gridSize());
        qDebug() << __FILE__ << __LINE__ << "GlyphSize: " << m_glyphEntry.glyphSize();
        renderGlyphPixels();
        renderGlyphImage();
    }
}

void GlyphWidget::setCharacter(const QChar &newCharacter) {
    m_glyphKey.setCharacter(newCharacter);
    if(m_glyphKey.isValid()) {
        updateGlyph();
        update();
    }
}

void GlyphWidget::setGlyphSize(int newGlyphSize) {
    m_glyphEntry.setGlyphSize(newGlyphSize);
    qDebug() << __FUNCTION__ << newGlyphSize;
    if (m_glyphKey.isValid() && m_glyphEntry.isValid()) {
        updateGlyph();
        update();
    }
}

void GlyphWidget::setGlyphFont(const QFont &newFont, const QString &newFontPath)
{
    QString oldFontPath = m_glyphEntry.fontPath();
    m_glyphKey.setFont(newFont);
    m_glyphEntry.setFontPath(newFontPath);

    if (m_ftFace) {
        FT_Done_Face(m_ftFace);
        m_ftFace = 0;
    }

    FT_Error ftErr = FT_Err_Ok;
    ftErr = FT_New_Face(m_ftLib, m_glyphEntry.fontPath().toLatin1().constData(), 0, &m_ftFace);
    if (!ftErr) {
        qDebug() << "Face Loaded";
    } else {
        qDebug() << "Error FT Face Load";
    }

    if (m_glyphKey.isValid() && m_glyphEntry.isValid()) {
        updateGlyph();
        update();
    }
}


void GlyphWidget::resizeEvent(QResizeEvent *event)
{
    if (m_glyphKey.isValid() && m_glyphEntry.isValid()) {
        updateGlyph();
        update();
    }
}

void GlyphWidget::setGridSize(int newGridSize) {
    m_glyphKey.setGridSize(newGridSize);
    if (m_glyphKey.isValid())
    {
        updateGlyph();
        update();
    }
}

void GlyphWidget::setGlyphParams(const QFont &newFont, const QString & newFontPath, int newGlyphSize, const QChar &newCharacter, int newGridSize)
{
    setGridSize(newGridSize);
    setGlyphSize(newGlyphSize);
    setCharacter(newCharacter);
    setGlyphSize(newGlyphSize);
    setGlyphFont(newFont, newFontPath);

    qDebug() << "Set Glyph Params: " << "Font Path: " << newFontPath;

    if (m_glyphKey.isValid() && m_glyphEntry.isValid()) {
        updateGlyph();
        update();
    }
}

int GlyphWidget::glyphKeyEntry ()
{
    auto it = m_glyphCache.find(m_glyphKey);
    if (it != m_glyphCache.end()) {
        // const GlyphEntry &entry = it.value();
        m_glyphEntry = it.value();
        return 1;
    }

    m_glyphCache.insert(m_glyphKey, m_glyphEntry);
    return 0;
}

void GlyphWidget::moveGlyphLeft()
{
    if (m_glyphKey.isValid() && m_glyphEntry.isValid())
    {
        m_glyphEntry.moveLeft();
        update();
    }
}

void GlyphWidget::moveGlyphTop()
{
    if (m_glyphKey.isValid() && m_glyphEntry.isValid())
    {
        m_glyphEntry.moveTop();
        update();
    }
}

void GlyphWidget::moveGlyphDown()
{
    if (m_glyphKey.isValid() && m_glyphEntry.isValid())
    {
        m_glyphEntry.moveDown();
        update();
    }
}

void GlyphWidget::moveGlyphRight()
{
    if (m_glyphKey.isValid() && m_glyphEntry.isValid())
    {
        m_glyphEntry.moveRight();
        update();
    }
}

void GlyphWidget::moveGlyphCenter()
{
    if (m_glyphKey.isValid() && m_glyphEntry.isValid())
    {
        m_glyphEntry.moveCenter();
        update();
    }
}
