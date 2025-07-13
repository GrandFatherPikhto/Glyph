#include "glyphwidget.h"
#include "ui_glyphwidget.h"

GlyphWidget::GlyphWidget(GlyphManager *glyphManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GlyphWidget)
    , m_glyphManager(glyphManager)
    , m_glyphImage(nullptr)
    , m_ftLib(0)
    , m_ftFace(0)
    , m_glyph(nullptr)
{
    ui->setupUi(this);
    FT_Error ftErr = FT_Err_Ok;
    ftErr = FT_Init_FreeType(&m_ftLib);
}

GlyphWidget::~GlyphWidget()
{
    delete ui;

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
        FT_Load_Char(m_ftFace, m_glyph->character().unicode(), FT_LOAD_DEFAULT);

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

void GlyphWidget::calcRenderRect ()
{
    if (m_glyph && m_glyph->isValid())
    {
        // m_m_renderRect.left() = (width() - m_glyph->gridSize() * m_gridCellSize) / 2;
        // m_m_renderRect.top() = (height() - m_glyph->gridSize() * m_gridCellSize) / 2;
        // m_left = m_renderRect.left() + m_glyph->glyphLeft() * m_gridCellSize;
        // m_top = m_renderRect.top() + m_glyph->glyphTop() * m_gridCellSize;
        m_renderRect.setLeft((width() - m_glyph->gridSize() * m_gridCellSize) / 2);
        m_renderRect.setTop((height() - m_glyph->gridSize() * m_gridCellSize) / 2);
        m_renderRect.setWidth(m_glyph->gridSize() * m_gridCellSize);
        m_renderRect.setHeight(m_glyph->gridSize() * m_gridCellSize);

        m_glyphRect.setLeft(m_renderRect.left() + (m_glyph->glyphRect().left()) * m_gridCellSize);
        m_glyphRect.setTop(m_renderRect.top() + m_glyph->glyphRect().top() * m_gridCellSize);
        m_glyphRect.setWidth(m_glyph->glyphRect().width() * m_gridCellSize);
        m_glyphRect.setHeight(m_glyph->glyphRect().height() * m_gridCellSize);

        qDebug() << m_glyphRect;
    }
}


void GlyphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (!m_glyph || !m_glyph->isValid() || !m_glyph->glyphPixelsIsValid())
        return;

    calcRenderRect();

    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);

    if (m_gridEnable)
    {
        // Горизонтальные линии
        for (int y = 0; y <= m_glyph->gridSize(); ++y) {
            painter.drawLine(
                m_renderRect.left(),
                m_renderRect.top() + y * m_gridCellSize,
                m_renderRect.left() + m_glyph->gridSize() * m_gridCellSize,
                m_renderRect.top() + y * m_gridCellSize
                );
        }

        // Вертикальные линии
        for (int x = 0; x <= m_glyph->gridSize(); ++x) {
            painter.drawLine(
                m_renderRect.left() + x * m_gridCellSize,
                m_renderRect.top(),
                m_renderRect.left() + x * m_gridCellSize,
                m_renderRect.top() + m_glyph->gridSize() * m_gridCellSize
                );
        }

    }

    if (m_countourEnable && m_glyphImage != nullptr)
    {
        QImage glyphImage = m_glyphImage->scaled(m_gridCellSize * m_glyph->glyphCols(), m_gridCellSize * m_glyph->glyphRows(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawImage(
            m_glyphRect.left(),
            m_glyphRect.top(),
            glyphImage
            );
    }

    // qDebug()() << "Pixels Vector Size: " << m_glyphPixels.size() << ((m_glyphPixels.size() == m_glyphRows * m_glyphCols) ? "=" : "≠") << m_glyphRows * m_glyphCols;
    if (m_generatedGlyphEnable && m_glyph->glyphPixelsIsValid()) {
        for(qint32 y = 0; y < m_glyph->glyphRows(); ++y) {
            for(qint32 x = 0; x < m_glyph->glyphCols(); ++x) {
                // Получаем значение пикселя (true - закрашен, false - пустой)
                // bool pixel = m_glyphPixels[y * m_glyphCols + x];
                bool pixel = m_glyph->getPixel(x, y);

                // Рассчитываем координаты квадрата с учетом масштаба
                QRect pixelRect(
                    m_glyphRect.left() + x * m_gridCellSize,
                    m_glyphRect.top() + y * m_gridCellSize,
                    m_gridCellSize,
                    m_gridCellSize
                    );

                // Отрисовываем квадрат
                if(pixel) {
                    painter.fillRect(pixelRect, QColor(0x33, 0x33, 0xFF , 0xEF));
                } else {
                    if (m_glyphGridEnable)
                    {
                        painter.fillRect(pixelRect, QColor(0x33, 0, 0, 0x11));
                        painter.setPen(Qt::black);
                        painter.drawRect(pixelRect);
                    }
                }
            }
        }
    }

    painter.end();
}

void GlyphWidget::resetGlyphPixels ()
{

}

void GlyphWidget::renderGlyphPixels ()
{
    FT_Error ftError = FT_Err_Ok;
    // if (m_glyph->glyphPixelsIsValid())
    //     return;
    // resetGlyphPixels();

    if (m_ftFace == 0) {
        qDebug() << "Font Face Not inited";
        return;
    }

    if (!m_glyph || !m_glyph->isValid())
        return;

    FT_UInt glyph_index = 0;
    ftError = FT_Set_Pixel_Sizes(m_ftFace, 0, m_glyph->glyphSize());
    if (!ftError) {
        // qDebug() << "Set pixel sizes to " << m_glyphSize;
    } else {
        qDebug() << "Can't set pixel sizes to " << m_glyph->character();
    }

    glyph_index = FT_Get_Char_Index(m_ftFace, m_glyph->character().unicode());

    ftError = FT_Load_Glyph(m_ftFace, glyph_index, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);

    QRect glyphRect(QPoint(m_ftFace->glyph->bitmap_left, m_glyph->gridSize() - m_ftFace->glyph->bitmap_top), QSize(m_ftFace->glyph->bitmap.width, m_ftFace->glyph->bitmap.rows));
    m_glyph->setGlyphRect(glyphRect);
    // qDebug() << "Glyph Rect: " << glyphRect;

    if (!ftError)
    {
        // setFixedSize(m_glyphRect.width(),
        //              m_glyphRect.height());
        // qDebug() << "Loaded Glyph " << m_character << m_ftFace->glyph->bitmap.width << m_ftFace->glyph->bitmap.rows;

        FT_Bitmap* bitmap = &m_ftFace->glyph->bitmap;
        // qDebug() << "Columns: " << bitmap->width << ", Rows: " << bitmap->rows;
        m_glyph->resetGlyphPixels();
        if (bitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
            for (int y = 0; y < bitmap->rows; y++) {
                for (int x = 0; x < bitmap->width; x++) {
                    // Получаем байт, содержащий пиксель
                    unsigned char byte = bitmap->buffer[y * bitmap->pitch + (x / 8)];
                    // Извлекаем бит (MSB → x=0, LSB → x=7)
                    unsigned char bit = (byte >> (7 - (x % 8))) & 0x1;
                    // printf("%c", bit ? '#' : ' ');
                    // m_glyphPixels.append(bit ? true : false);
                    m_glyph->addPixel(bit ? true : false);
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

    if (!m_glyph || !m_glyph->isValid())
        return;

    // int glyphSize = findOptimalGlyphSize();
    // qDebug()() << "Optimal Font Size: " << glyphSize << m_glyphCols * m_gridCellSize << "x" << m_glyphRows * m_gridCellSize;
    // Устанавливаем размер и загружаем глиф
    int glyphWidth = m_glyph->glyphCols() * m_gridCellSize * 2;
    int glyphHeight = m_glyph->glyphRows() * m_gridCellSize * 2;
    FT_Set_Pixel_Sizes(m_ftFace, glyphWidth, glyphHeight);
#if 1
    FT_Load_Char(m_ftFace, m_glyph->character().unicode(), FT_LOAD_RENDER);
#else
    FT_Load_Char(m_ftFace, m_character.unicode(), FT_LOAD_RENDER | FT_LOAD_COLOR);
#endif

    FT_GlyphSlot glyph = m_ftFace->glyph;
    FT_Bitmap bitmap = glyph->bitmap;

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
    if (m_glyph && m_glyph->isValid()) {
        qDebug() << __FILE__ << __LINE__ << m_glyph->toString();
        m_gridCellSize = height() * 4 / (5 * m_glyph->gridSize());
        renderGlyphPixels();
        renderGlyphImage();
    }
}

// void GlyphWidget::setGlyphSize(int newGlyphSize) {
//     m_glyph->setGlyphSize(newGlyphSize);
//     qDebug() << __FUNCTION__ << newGlyphSize;
//     if (m_glyph && m_glyph->isValid()) {
//         updateGlyph();
//         update();
//     }
// }

void GlyphWidget::resizeEvent(QResizeEvent *event)
{
    if (m_glyph && m_glyph->isValid()) {
        updateGlyph();
        update();
    }
}

void GlyphWidget::setGlyph(QSharedPointer<Glyph> newGliph)
{
    m_glyph = newGliph;
    loadFontFace();
    updateGlyph();
    update();
}

void GlyphWidget::loadFontFace()
{
    if (!m_glyph || m_glyph->fontPath().isEmpty())
        return;

    if (m_ftFace) {
        FT_Done_Face(m_ftFace);
        m_ftFace = 0;
    }

    FT_Error ftErr = FT_Err_Ok;
    ftErr = FT_New_Face(m_ftLib, m_glyph->fontPath().toLatin1().constData(), 0, &m_ftFace);
    if (!ftErr) {
        qDebug() << "Face Loaded";
    } else {
        qDebug() << "Error FT Face Load";
    }

    if (!ftErr)
    {
        updateGlyph();
        update();
    }
}

void GlyphWidget::enableGrid(bool enable)
{
    m_gridEnable = enable;
    updateGlyph();
    update ();
}

void GlyphWidget::enableContour(bool enable)
{
    m_countourEnable = enable;
    updateGlyph();
    update ();
}

void GlyphWidget::enableGeneratedGlyph(bool enable)
{
    m_generatedGlyphEnable = enable;
    updateGlyph();
    update ();
}

void GlyphWidget::enableGlyphGrid(bool enable)
{
    m_glyphGridEnable = enable;
    updateGlyph();
    update ();
}


void GlyphWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_glyph && m_glyph->glyphPixelsIsValid())
    {

    }
}
