
#pragma once

#include "carla/client/Actor.h"
#include "carla/client/ActorList.h"
#include "carla/client/DebugHelper.h"
#include "carla/client/Vehicle.h"
#include "carla/client/World.h"
#include "carla/geom/Location.h"
#include "carla/road/RoadTypes.h"
#include "carla/rpc/ActorId.h"

#include "carla/trafficmanager/SimpleWaypoint.h"

namespace carla
{
namespace traffic_manager
{

namespace cc = carla::client;
namespace cg = carla::geom;

using Actor = carla::SharedPtr<cc::Actor>;
using ActorId = carla::ActorId;
using ActorIdSet = std::unordered_set<ActorId>;
using SimpleWaypointPtr = std::shared_ptr<SimpleWaypoint>;
using Buffer = std::deque<SimpleWaypointPtr>;
using GeoGridId = carla::road::JuncId;

class TrackTraffic
{

private:
    /// Structure to keep track of overlapping waypoints between vehicles.
    using WaypointOverlap = std::unordered_map<uint64_t, ActorIdSet>;
    WaypointOverlap waypoint_overlap_tracker;

    /// Structure to keep track of waypoints occupied by vehicles;
    using WaypointIdSet = std::unordered_set<uint64_t>;
    using WaypointOccupancyMap = std::unordered_map<ActorId, WaypointIdSet>;
    WaypointOccupancyMap waypoint_occupied;

    /// Geodesic grids occupied by actors's paths.
    std::unordered_map<ActorId, std::unordered_set<GeoGridId>> actor_to_grids;
    /// Actors currently passing through grids.
    std::unordered_map<GeoGridId, ActorIdSet> grid_to_actors;

public:
    TrackTraffic();

    /// Methods to update, remove and retrieve vehicles passing through a waypoint.
    void UpdatePassingVehicle(uint64_t waypoint_id, ActorId actor_id);
    void RemovePassingVehicle(uint64_t waypoint_id, ActorId actor_id);
    ActorIdSet GetPassingVehicles(uint64_t waypoint_id) const;

    void UpdateGridPosition(const ActorId actor_id, const Buffer &buffer);
    void UpdateUnregisteredGridPosition(const ActorId actor_id,
                                        const std::vector<SimpleWaypointPtr> waypoints);

    ActorIdSet GetOverlappingVehicles(ActorId actor_id) const;
    /// Method to delete actor data from tracking.
    void DeleteActor(ActorId actor_id);

    std::unordered_set<GeoGridId> GetGridIds(ActorId actor_id) const;

    std::unordered_map<GeoGridId, ActorIdSet> GetGridActors() const;

    void Clear();
};

} // namespace traffic_manager
} // namespace carla