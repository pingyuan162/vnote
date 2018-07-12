#ifndef MARKDOWNHIGHLIGHTERDATA_H
#define MARKDOWNHIGHLIGHTERDATA_H

#include <QTextCharFormat>

#include "vconstants.h"
#include "vtextblockdata.h"

extern "C" {
#include <pmh_parser.h>
}

struct HighlightingStyle
{
    pmh_element_type type;
    QTextCharFormat format;
};

// One continuous region for a certain markdown highlight style
// within a QTextBlock.
// Pay attention to the change of HighlightingStyles[]
struct HLUnit
{
    // Highlight offset @start and @length with style HighlightingStyles[styleIndex]
    // within a QTextBlock
    unsigned long start;
    unsigned long length;
    unsigned int styleIndex;
};

struct HLUnitStyle
{
    unsigned long start;
    unsigned long length;
    QString style;
};

// Fenced code block only.
struct VCodeBlock
{
    // Global position of the start.
    int m_startPos;

    int m_startBlock;
    int m_endBlock;

    QString m_lang;

    QString m_text;

    bool equalContent(const VCodeBlock &p_block) const
    {
        return p_block.m_lang == m_lang && p_block.m_text == m_text;
    }

    void updateNonContent(const VCodeBlock &p_block)
    {
        m_startPos = p_block.m_startPos;
        m_startBlock = p_block.m_startBlock;
        m_endBlock = p_block.m_endBlock;
    }
};


struct VMathjaxBlock
{
    VMathjaxBlock()
        : m_blockNumber(-1),
          m_previewedAsBlock(false),
          m_index(-1),
          m_length(-1)
    {
    }

    VMathjaxBlock(int p_blockNumber, const MathjaxInfo &p_info)
        : m_blockNumber(p_blockNumber),
          m_previewedAsBlock(p_info.m_previewedAsBlock),
          m_index(p_info.m_index),
          m_length(p_info.m_length),
          m_text(p_info.m_text)
    {
    }

    bool equalContent(const VMathjaxBlock &p_block) const
    {
        return m_text == p_block.m_text;
    }

    void updateNonContent(const VMathjaxBlock &p_block)
    {
        m_blockNumber = p_block.m_blockNumber;
        m_previewedAsBlock = p_block.m_previewedAsBlock;
        m_index = p_block.m_index;
        m_length = p_block.m_length;
    }

    int m_blockNumber;

    bool m_previewedAsBlock;

    // Start index within the block.
    int m_index;

    int m_length;

    QString m_text;
};


// Highlight unit with global position and string style name.
struct HLUnitPos
{
    HLUnitPos() : m_position(-1), m_length(-1)
    {
    }

    HLUnitPos(int p_position, int p_length, const QString &p_style)
        : m_position(p_position), m_length(p_length), m_style(p_style)
    {
    }

    int m_position;
    int m_length;
    QString m_style;
};

// Denote the region of a certain Markdown element.
struct VElementRegion
{
    VElementRegion() : m_startPos(0), m_endPos(0) {}

    VElementRegion(int p_start, int p_end) : m_startPos(p_start), m_endPos(p_end) {}

    // The start position of the region in document.
    int m_startPos;

    // The end position of the region in document.
    int m_endPos;

    // Whether this region contains @p_pos.
    bool contains(int p_pos) const
    {
        return m_startPos <= p_pos && m_endPos > p_pos;
    }

    bool intersect(int p_start, int p_end) const
    {
        return !(p_end <= m_startPos || p_start >= m_endPos);
    }

    bool operator==(const VElementRegion &p_other) const
    {
        return (m_startPos == p_other.m_startPos
                && m_endPos == p_other.m_endPos);
    }

    bool operator<(const VElementRegion &p_other) const
    {
        if (m_startPos < p_other.m_startPos) {
            return true;
        } else if (m_startPos == p_other.m_startPos) {
            // If a < b is true, then b < a must be false.
            return m_endPos < p_other.m_endPos;
        } else {
            return false;
        }
    }

    QString toString() const
    {
        return QString("[%1,%2)").arg(m_startPos).arg(m_endPos);
    }
};

struct PegHighlightResult
{
    TimeStamp m_timeStamp;

    QVector<QVector<HLUnit> > m_blockHighlights;
};

#endif // MARKDOWNHIGHLIGHTERDATA_H
