#include "TestHelpers.h"

#include <gtest/gtest.h>

#include "transport/analytics/CustomerFeedback.h"
#include "transport/analytics/FuelConsumption.h"
#include "transport/analytics/Metrics.h"
#include "transport/analytics/PerformanceMetric.h"
#include "transport/analytics/ServiceQualityMonitor.h"
#include "transport/analytics/TrafficInfo.h"
#include "transport/exceptions/InvalidRatingException.h"
#include "transport/exceptions/TransportException.h"

namespace {

using namespace transport;
using namespace transport::test;

TEST(PerformanceMetricTest, ComparesWithTargetAndUpdates) {
    PerformanceMetric metric("On-time delivery", 90.0, 80.0);
    EXPECT_TRUE(metric.isOnTarget());
    EXPECT_NEAR(metric.deviation(), 10.0, 1e-9);
    metric.update(70.0);
    EXPECT_FALSE(metric.isOnTarget());
    EXPECT_NEAR(metric.deviation(), -10.0, 1e-9);
    EXPECT_THROW(PerformanceMetric("", 1.0, 1.0), TransportException);
}

TEST(FuelConsumptionTest, ComputesConsumptionAndCost) {
    FuelConsumption fuel(200.0, 20.0, 50.0);
    EXPECT_NEAR(fuel.per100Km(), 10.0, 1e-9);
    EXPECT_NEAR(fuel.totalCost().amount(), 1000.0, 1e-9);
    EXPECT_TRUE(fuel.isEfficient(10.0));
    EXPECT_FALSE(fuel.isEfficient(9.0));
    EXPECT_THROW((void)fuel.isEfficient(0.0), TransportException);
}

TEST(FuelConsumptionTest, RejectsInvalidData) {
    EXPECT_THROW(FuelConsumption(0.0, 1.0, 1.0), TransportException);
    EXPECT_THROW(FuelConsumption(1.0, -1.0, 1.0), TransportException);
    EXPECT_THROW(FuelConsumption(1.0, 1.0, -1.0), TransportException);
}

TEST(TrafficInfoTest, AdjustsDurationAndDetectsSevereTraffic) {
    TrafficInfo traffic("Moscow", 30, 4);
    EXPECT_NEAR(traffic.delayFactor(), 1.5, 1e-9);
    EXPECT_NEAR(traffic.adjustedDuration(100.0), 150.0, 1e-9);
    EXPECT_TRUE(traffic.isSevere());
    EXPECT_THROW((void)traffic.adjustedDuration(-1.0), TransportException);

    TrafficInfo light("Moscow", 0, 1);
    EXPECT_FALSE(light.isSevere());
}

TEST(TrafficInfoTest, RejectsInvalidData) {
    EXPECT_THROW(TrafficInfo("", 0, 1), TransportException);
    EXPECT_THROW(TrafficInfo("Moscow", -1, 1), TransportException);
    EXPECT_THROW(TrafficInfo("Moscow", 0, 0), TransportException);
    EXPECT_THROW(TrafficInfo("Moscow", 0, 6), TransportException);
}

TEST(MetricsTest, AveragesAndSummarizes) {
    Metrics metrics(FuelConsumption(200.0, 20.0, 50.0),
                    TrafficInfo("Moscow", 30, 4));
    EXPECT_NEAR(metrics.averageValue(), 0.0, 1e-9);

    metrics.addMetric(PerformanceMetric("A", 80.0, 70.0));
    metrics.addMetric(PerformanceMetric("B", 60.0, 70.0));
    EXPECT_NEAR(metrics.averageValue(), 70.0, 1e-9);
    EXPECT_EQ(metrics.performanceMetrics().size(), 2);
    EXPECT_NE(metrics.summarize().find("average=70"), std::string::npos);
}

TEST(CustomerFeedbackTest, ClassifiesAndSummarizes) {
    const CustomerFeedback positive(1, 1, 5, "Great", date());
    EXPECT_TRUE(positive.isPositive());
    EXPECT_NE(positive.summary().find("5/5"), std::string::npos);

    const CustomerFeedback negative(1, 1, 3, "Average", date());
    EXPECT_FALSE(negative.isPositive());
}

TEST(CustomerFeedbackTest, RejectsInvalidData) {
    EXPECT_THROW(CustomerFeedback(0, 1, 5, "Great", date()), TransportException);
    EXPECT_THROW(CustomerFeedback(1, 0, 5, "Great", date()), TransportException);
    EXPECT_THROW(CustomerFeedback(1, 1, 0, "Great", date()), InvalidRatingException);
    EXPECT_THROW(CustomerFeedback(1, 1, 6, "Great", date()), InvalidRatingException);
    EXPECT_THROW(CustomerFeedback(1, 1, 5, "", date()), TransportException);
}

TEST(ServiceQualityMonitorTest, ComputesRatingsAndComplaints) {
    ServiceQualityMonitor monitor;
    EXPECT_NEAR(monitor.averageRating(), 0.0, 1e-9);
    EXPECT_NEAR(monitor.complaintRate(), 0.0, 1e-9);
    EXPECT_TRUE(monitor.topComplaints(1).empty());

    monitor.addFeedback(CustomerFeedback(1, 1, 5, "Great", date()));
    monitor.addFeedback(CustomerFeedback(1, 2, 2, "Late", date()));
    EXPECT_EQ(monitor.feedbackCount(), 2);
    EXPECT_NEAR(monitor.averageRating(), 3.5, 1e-9);
    EXPECT_NEAR(monitor.complaintRate(), 0.5, 1e-9);
    ASSERT_EQ(monitor.topComplaints(1).size(), 1);
    EXPECT_EQ(monitor.topComplaints(1)[0], "Late");
    EXPECT_TRUE(monitor.topComplaints(0).empty());
}

} // namespace
