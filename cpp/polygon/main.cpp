#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

struct Point
{
    double x, y;

    bool operator<(const Point &other) const
    {
        return x < other.x || (x == other.x && y < other.y);
    }
};

struct Segment
{
    Point start, end;

    bool operator<(const Segment &other) const
    {
        // Sort segments based on their y-coordinates
        return std::min(start.y, end.y) < std::min(other.start.y, other.end.y);
    }
};

class Event
{
public:
    enum Type
    {
        LEFT,
        RIGHT,
        INTERSECTION
    };

    Event(Type type, const Point &point, const Segment *segment)
        : type(type)
        , point(point)
        , segment(segment)
    {
    }

    Type type;
    Point point;
    const Segment *segment;

    bool operator<(const Event &other) const
    {
        return point.y < other.point.y || (point.y == other.point.y && type < other.type);
    }
};

class SweepLine
{
public:
    void processEvents(const std::vector<Point> &vertices)
    {
        std::vector<Event> events;
        std::vector<Segment> segments;

        // Create segments from the input vertices
        for (size_t i = 0; i < vertices.size() - 1; ++i)
        {
            segments.emplace_back(Segment{ vertices[i], vertices[i + 1] });
        }
        segments.emplace_back(Segment{ vertices.back(), vertices.front() });

        // Create events from segments
        for (const auto &segment : segments)
        {
            events.emplace_back(Event::LEFT, segment.start, &segment);
            events.emplace_back(Event::RIGHT, segment.end, &segment);
        }

        std::sort(events.begin(), events.end());

        std::set<const Segment *> activeSegments;

        for (const auto &event : events)
        {
            switch (event.type)
            {
            case Event::LEFT:
                for (const Segment *s : activeSegments)
                {
                    if (doIntersect(event.segment, s))
                    {
                        std::cout << "Polygon is self-intersecting.\n";
                        return;
                    }
                }
                activeSegments.insert(event.segment);
                break;

            case Event::RIGHT:
                activeSegments.erase(event.segment);
                break;

            case Event::INTERSECTION:
                std::cout << "Polygon is self-intersecting.\n";
                return;
            }
        }

        std::cout << "Polygon is not self-intersecting.\n";
    }

private:
    bool doIntersect(const Segment *s1, const Segment *s2) const
    {
        // Implementation of line segment intersection test
        // (Note: This implementation assumes that line segments are not collinear)
        double x1 = s1->start.x, y1 = s1->start.y;
        double x2 = s1->end.x, y2 = s1->end.y;
        double x3 = s2->start.x, y3 = s2->start.y;
        double x4 = s2->end.x, y4 = s2->end.y;

        double d1 = orientation(x3, y3, x4, y4, x1, y1);
        double d2 = orientation(x3, y3, x4, y4, x2, y2);
        double d3 = orientation(x1, y1, x2, y2, x3, y3);
        double d4 = orientation(x1, y1, x2, y2, x4, y4);

        if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
        {
            return true; // Segments intersect
        }

        return false;
    }

    double orientation(double x1, double y1, double x2, double y2, double x3, double y3) const
    {
        return (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
    }
};

int main()
{
    // Example usage
    std::vector<Point> vertices = { { 0, 0 }, { 1, 1 }, { 2, 0 }, { 1, -1 } };

    SweepLine sweepLine;
    sweepLine.processEvents(vertices);

    return 0;
}
