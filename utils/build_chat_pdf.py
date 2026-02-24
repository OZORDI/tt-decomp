#!/usr/bin/env python3
"""Render a PDF transcript from /tmp/messages.json entirely on the host."""
from __future__ import annotations

import json
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from textwrap import wrap

from reportlab.lib import colors
from reportlab.lib.pagesizes import LETTER
from reportlab.lib.units import inch
from reportlab.pdfgen import canvas

# Paths
SOURCE_PATH = Path("/tmp/messages.json")
OUTPUT_PATH = Path("rent_chat_transcript.pdf")

# Layout constants
PAGE_WIDTH, PAGE_HEIGHT = LETTER
MARGIN = 0.75 * inch
MAX_BUBBLE_WIDTH = PAGE_WIDTH - (2 * MARGIN) - 0.5 * inch
LINE_HEIGHT = 14
TIME_GAP_MINUTES = 15

# Colors
BLUE = colors.Color(0.21, 0.52, 0.96)
GRAY = colors.Color(0.9, 0.9, 0.9)

FONT_REGULAR = "Helvetica"
FONT_BOLD = "Helvetica-Bold"


@dataclass
class Message:
    from_me: bool
    text: str
    timestamp: datetime


def load_messages() -> list[Message]:
    serialized = json.loads(SOURCE_PATH.read_text())
    messages: list[Message] = []
    for item in serialized:
        dt = datetime.fromisoformat(item["dt"])
        messages.append(
            Message(
                from_me=bool(item["from_me"]),
                text=item["text"].strip(),
                timestamp=dt,
            )
        )
    return messages


def maybe_draw_timestamp(c: canvas.Canvas, last: datetime | None, current: datetime, y: float) -> float:
    if last and (current - last).total_seconds() < TIME_GAP_MINUTES * 60:
        return y
    timestamp = current.strftime("%b %d, %Y • %I:%M %p")
    ts_width = c.stringWidth(timestamp, FONT_REGULAR, 9) + 12
    ts_height = 14
    x = (PAGE_WIDTH - ts_width) / 2
    c.setFillColor(colors.gray)
    c.roundRect(x, y - ts_height, ts_width, ts_height, 7, stroke=0, fill=1)
    c.setFillColor(colors.white)
    c.setFont(FONT_REGULAR, 9)
    c.drawCentredString(PAGE_WIDTH / 2, y - ts_height + 3, timestamp)
    c.setFont(FONT_REGULAR, 11)
    return y - ts_height - 12


def draw_bubble(
    c: canvas.Canvas,
    is_me: bool,
    text: str,
    y: float,
) -> float:
    lines = wrap(text, max(1, int(MAX_BUBBLE_WIDTH / 6))) or [text]
    padding = 8
    width = max(c.stringWidth(line, FONT_REGULAR, 11) for line in lines) + 2 * padding
    height = len(lines) * LINE_HEIGHT + 2 * padding
    x = PAGE_WIDTH - MARGIN - width if is_me else MARGIN

    bubble_color = BLUE if is_me else GRAY
    c.setFillColor(bubble_color)
    c.roundRect(x, y - height, width, height, 8, stroke=0, fill=1)

    text_color = colors.white if is_me else colors.black
    c.setFillColor(text_color)
    text_y = y - padding - LINE_HEIGHT + 4
    for line in lines:
        c.drawString(x + padding, text_y, line)
        text_y -= LINE_HEIGHT

    return y - height - 10


def render(messages: list[Message]) -> None:
    c = canvas.Canvas(str(OUTPUT_PATH), pagesize=LETTER)
    c.setTitle("Rent Chat Transcript")
    c.setAuthor("Local Export")

    y = PAGE_HEIGHT - MARGIN
    c.setFont(FONT_BOLD, 16)
    c.drawString(MARGIN, y, "Prince ↔ Landlord")
    y -= 30
    c.setFont(FONT_REGULAR, 11)

    last_timestamp: datetime | None = None

    for message in messages:
        y = maybe_draw_timestamp(c, last_timestamp, message.timestamp, y)
        last_timestamp = message.timestamp

        if y < MARGIN + 80:
            c.showPage()
            y = PAGE_HEIGHT - MARGIN
            c.setFont(FONT_REGULAR, 11)

        y = draw_bubble(c, message.from_me, message.text, y)

        if y < MARGIN + 50:
            c.showPage()
            y = PAGE_HEIGHT - MARGIN
            c.setFont(FONT_REGULAR, 11)

    c.save()


def main() -> None:
    messages = load_messages()
    render(messages)
    print(f"Saved PDF to {OUTPUT_PATH.resolve()}")


if __name__ == "__main__":
    main()
