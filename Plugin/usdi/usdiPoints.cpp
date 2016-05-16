#include "pch.h"
#include "usdiInternal.h"
#include "usdiSchema.h"
#include "usdiXform.h"
#include "usdiPoints.h"

namespace usdi {


Points::Points(Context *ctx, Schema *parent, const UsdGeomPoints& points)
    : super(ctx, parent, points)
    , m_points(points)
{
    usdiTrace("Points::Points(): %s\n", getPath());
}

Points::~Points()
{
    usdiTrace("Points::~Points(): %s\n", getPath());
}

UsdGeomPoints& Points::getUSDSchema()
{
    return m_points;
}

void Points::getSummary(PointsSummary& dst) const
{
    // todo
}

bool Points::readSample(PointsData& dst, Time t_)
{
    auto t = (const UsdTimeCode&)t_;
    const auto& conf = getImportConfig();

    PointsSample sample;
    m_points.GetPointsAttr().Get(&sample.points, t);
    m_points.GetVelocitiesAttr().Get(&sample.velocities, t);


    dst.num_points = sample.points.size();
    if (dst.points) {
        memcpy(dst.points, &sample.points[0], sizeof(float3) * dst.num_points);
        if (conf.swap_handedness) {
            for (uint i = 0; i < dst.num_points; ++i) {
                dst.points[i].x *= -1.0f;
            }
        }
    }
    if (dst.velocities) {
        memcpy(dst.velocities, &sample.velocities[0], sizeof(float3) * dst.num_points);
        if (conf.swap_handedness) {
            for (uint i = 0; i < dst.num_points; ++i) {
                dst.velocities[i].x *= -1.0f;
            }
        }
    }

    return dst.num_points > 0;
}

bool Points::writeSample(const PointsData& src, Time t_)
{
    auto t = (const UsdTimeCode&)t_;
    const auto& conf = getExportConfig();

    PointsSample sample;

    if (src.points) {
        sample.points.assign((GfVec3f*)src.points, (GfVec3f*)src.points + src.num_points);
        if (conf.swap_handedness) {
            for (auto& v : sample.points) {
                v[0] *= -1.0f;
            }
        }
    }

    if (src.velocities) {
        sample.points.assign((GfVec3f*)src.velocities, (GfVec3f*)src.velocities + src.num_points);
        if (conf.swap_handedness) {
            for (auto& v : sample.velocities) {
                v[0] *= -1.0f;
            }
        }
    }

    bool  ret = m_points.GetPointsAttr().Set(sample.points, t);
    m_points.GetVelocitiesAttr().Set(sample.velocities, t);
    return ret;
}

} // namespace usdi