#include "gtest/gtest.h"
#include "CustomTime.h"
#include "Event.h"
#include "ClubManager.h"
#include "TableManager.h"

// CustomTime.h тесты
TEST(TimeFormatTest, ValidFormats) {
    EXPECT_TRUE(CustomTime::isValidFormat("00:00"));
    EXPECT_TRUE(CustomTime::isValidFormat("23:59"));
    EXPECT_TRUE(CustomTime::isValidFormat("09:05"));
}

TEST(TimeFormatTest, InvalidFormats) {
    EXPECT_FALSE(CustomTime::isValidFormat("24:00"));  
    EXPECT_FALSE(CustomTime::isValidFormat("12:60"));  
    EXPECT_FALSE(CustomTime::isValidFormat("12.30"));  
    EXPECT_FALSE(CustomTime::isValidFormat("1230"));   
    EXPECT_FALSE(CustomTime::isValidFormat("1:30")); 
    EXPECT_FALSE(CustomTime::isValidFormat("12:3")); 
    EXPECT_FALSE(CustomTime::isValidFormat(""));  
    EXPECT_FALSE(CustomTime::isValidFormat("aa:bb"));  
}

TEST(ClientFormatTest, ValidFormats) {
    EXPECT_TRUE(Event::isValidClientName("client1"));
    EXPECT_TRUE(Event::isValidClientName("clie3108fhe_31f-"));
    EXPECT_TRUE(Event::isValidClientName("----"));
    EXPECT_TRUE(Event::isValidClientName("f3j9180"));
    EXPECT_TRUE(Event::isValidClientName("_f3--_"));
    EXPECT_TRUE(Event::isValidClientName("1"));
}

TEST(ClientFormatTest, InvalidFormats) {
    EXPECT_FALSE(Event::isValidClientName(""));  
    EXPECT_FALSE(Event::isValidClientName("."));
    EXPECT_FALSE(Event::isValidClientName("cliFEFnt1"));  
    EXPECT_FALSE(Event::isValidClientName("CLIENT1"));  
    EXPECT_FALSE(Event::isValidClientName("cleinr1_3 4"));  
    EXPECT_FALSE(Event::isValidClientName("CLIEN ffef"));  
}

TEST(ClientTableFormatTest, ValidFormats) {
    EXPECT_TRUE(Event::isValidTableEventBody("client1 3"));
    EXPECT_TRUE(Event::isValidTableEventBody("client1fe-0e-f-__- 3"));
    EXPECT_TRUE(Event::isValidTableEventBody("3 3"));
}

TEST(ClientTableFormatTest, InvalidFormats) {
    EXPECT_FALSE(Event::isValidTableEventBody(" 3"));
    EXPECT_FALSE(Event::isValidTableEventBody("f1c__-13f"));
    EXPECT_FALSE(Event::isValidTableEventBody(""));
    EXPECT_FALSE(Event::isValidTableEventBody("____3_ "));
    EXPECT_FALSE(Event::isValidTableEventBody("3ofj0 A"));
}

// Event.h тесты
TEST(EventTest, ValidEventParsing) {
    Event e = Event::parse("08:48 1 client1", 3, CustomTime(0,0));
    EXPECT_EQ(e.getId(), 1);
    EXPECT_EQ(e.getClient(), "client1");
    EXPECT_EQ(e.getTable(), 0);
    Event e1 = Event::parse("00:00 2 client1 4", 5, CustomTime(0,0));
    EXPECT_EQ(e1.getId(), 2);
    EXPECT_EQ(e1.getClient(), "client1");
    EXPECT_EQ(e1.getTable(), 4);
}

// ClubManager.h тесты
class ClubManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        manager = new ClubManager(3, 10, CustomTime(9,0), CustomTime(19,0));
    }
    
    void TearDown() override {
        delete manager;
    }
    
    ClubManager* manager;
};

TEST_F(ClubManagerTest, ClientArrivalBeforeOpen) {
    Event e = Event::parse("08:00 1 client1", 3, CustomTime(0,0));
    testing::internal::CaptureStdout();
    manager->processEvent(e);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("08:00 13 NotOpenYet"), std::string::npos);
}

// TableManager.h тесты
TEST(TableManagerTest, TableOccupation) {
    TableManager tm(3, 10);
    Event e(CustomTime(10,0), 2, "client1", 1);
    
    tm.occupyTable(1, e);
    EXPECT_FALSE(tm.isTableFree(1));
    EXPECT_EQ(tm.getFreeTableCount(), 2);
}

TEST(EdgeCasesTest, MidnightClub) {
    ClubManager manager(2, 15, CustomTime(23,0), CustomTime(23,59));
    manager.processEvent(Event::parse("23:30 1 client1", 2, CustomTime(0,0)));
    
    testing::internal::CaptureStdout();
    manager.closeClub();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("23:59 11 client1"), std::string::npos);
}


// ==================== ID 1: Клиент пришел ====================
TEST_F(ClubManagerTest, ClientArrivalWhenAlreadyInClub) {
    manager->processEvent(Event::parse("09:01 1 client1", 3, CustomTime(0,0)));
    testing::internal::CaptureStdout();
    manager->processEvent(Event::parse("09:02 1 client1", 3, CustomTime(9,1)));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("09:02 13 YouShallNotPass"), std::string::npos);
}

TEST_F(ClubManagerTest, ClientArrivalAtExactOpenTime) {
    EXPECT_NO_THROW(manager->processEvent(Event::parse("09:00 1 client1", 3, CustomTime(0,0))));
}

// ==================== ID 2: Клиент сел за стол ====================
TEST_F(ClubManagerTest, ClientSitsAtTableSuccess) {
    manager->processEvent(Event::parse("09:01 1 client1", 3, CustomTime(0,0)));
    EXPECT_NO_THROW(manager->processEvent(Event::parse("09:02 2 client1 1", 3, CustomTime(9,1))));
}

TEST_F(ClubManagerTest, ClientChangesTable) {
    manager->processEvent(Event::parse("09:01 1 client1", 3, CustomTime(0,0)));
    manager->processEvent(Event::parse("09:02 2 client1 1", 3, CustomTime(9,1)));
    testing::internal::CaptureStdout();
    manager->processEvent(Event::parse("09:03 2 client1 2", 3, CustomTime(9,2)));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("13"), std::string::npos); // Нет ошибок
}

TEST_F(ClubManagerTest, SitAtOccupiedTable) {
    manager->processEvent(Event::parse("09:01 1 client1", 3, CustomTime(0,0)));
    manager->processEvent(Event::parse("09:02 1 client2", 3, CustomTime(9,1)));
    manager->processEvent(Event::parse("09:03 2 client1 1", 3, CustomTime(9,2)));
    testing::internal::CaptureStdout();
    manager->processEvent(Event::parse("09:04 2 client2 1", 3, CustomTime(9,3)));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("09:04 13 PlaceIsBusy"), std::string::npos);
}

// ==================== ID 3: Клиент ожидает ====================
TEST_F(ClubManagerTest, ClientWaitsWhenNoFreeTables) {
    ClubManager smallClub(1, 10, CustomTime(9,0), CustomTime(19,0));
    smallClub.processEvent(Event::parse("09:01 1 client1", 1, CustomTime(0,0)));
    smallClub.processEvent(Event::parse("09:02 2 client1 1", 1, CustomTime(9,1)));
    smallClub.processEvent(Event::parse("09:03 1 client2", 1, CustomTime(9,2)));
    testing::internal::CaptureStdout();
    smallClub.processEvent(Event::parse("09:04 3 client2", 1, CustomTime(9,3)));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("13"), std::string::npos); // Нет ошибок
}

TEST_F(ClubManagerTest, ClientCannotWaitWhenFreeTables) {
    manager->processEvent(Event::parse("09:01 1 client1", 3, CustomTime(0,0)));
    testing::internal::CaptureStdout();
    manager->processEvent(Event::parse("09:02 3 client1", 3, CustomTime(9,1)));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("09:02 13 ICanWaitNoLonger!"), std::string::npos);
}

TEST_F(ClubManagerTest, ClientLeavesWhenQueueFull) {
    ClubManager smallClub(1, 10, CustomTime(9,0), CustomTime(19,0));
    smallClub.processEvent(Event::parse("09:01 1 client1", 1, CustomTime(0,0)));
    smallClub.processEvent(Event::parse("09:02 2 client1 1", 1, CustomTime(9,1)));
    smallClub.processEvent(Event::parse("09:03 1 client2", 1, CustomTime(9,2)));
    smallClub.processEvent(Event::parse("09:04 3 client2", 1, CustomTime(9,3)));
    testing::internal::CaptureStdout();
    smallClub.processEvent(Event::parse("09:05 1 client3", 1, CustomTime(9,4)));
    smallClub.processEvent(Event::parse("09:06 3 client3", 1, CustomTime(9,5)));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("09:06 11 client3"), std::string::npos);
}

// ==================== ID 4: Клиент ушел ====================
TEST_F(ClubManagerTest, ClientLeavesSuccess) {
    manager->processEvent(Event::parse("09:01 1 client1", 3, CustomTime(0,0)));
    manager->processEvent(Event::parse("09:02 2 client1 1", 3, CustomTime(9,1)));
    EXPECT_NO_THROW(manager->processEvent(Event::parse("09:03 4 client1", 3, CustomTime(9,2))));
}

TEST_F(ClubManagerTest, ClientLeavesUnknown) {
    testing::internal::CaptureStdout();
    manager->processEvent(Event::parse("09:01 4 client1", 3, CustomTime(0,0)));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("09:01 13 ClientUnknown"), std::string::npos);
}

// ==================== Исходящие события ====================
TEST_F(ClubManagerTest, OutgoingEvent11_ClientForcedToLeave) {
    manager->processEvent(Event::parse("09:01 1 client1", 3, CustomTime(0,0)));
    testing::internal::CaptureStdout();
    manager->closeClub();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("19:00 11 client1"), std::string::npos);
}

TEST_F(ClubManagerTest, OutgoingEvent12_ClientFromQueueSits) {
    ClubManager smallClub(1, 10, CustomTime(9,0), CustomTime(19,0));
    smallClub.processEvent(Event::parse("09:01 1 client1", 1, CustomTime(0,0)));
    smallClub.processEvent(Event::parse("09:02 2 client1 1", 1, CustomTime(9,1)));
    smallClub.processEvent(Event::parse("09:03 1 client2", 1, CustomTime(9,2)));
    smallClub.processEvent(Event::parse("09:04 3 client2", 1, CustomTime(9,3)));
    testing::internal::CaptureStdout();
    smallClub.processEvent(Event::parse("09:05 4 client1", 1, CustomTime(9,4)));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("09:05 12 client2 1"), std::string::npos);
}



TEST_F(ClubManagerTest, ComplexScenario1) {
    manager->processEvent(Event::parse("09:01 1 client1", 3, CustomTime(0,0)));
    manager->processEvent(Event::parse("09:02 1 client2", 3, CustomTime(9,1)));
    manager->processEvent(Event::parse("09:03 2 client1 1", 3, CustomTime(9,2)));
    manager->processEvent(Event::parse("09:04 2 client2 2", 3, CustomTime(9,3)));
    manager->processEvent(Event::parse("10:00 1 client3", 3, CustomTime(9,4)));
    testing::internal::CaptureStdout();
    manager->processEvent(Event::parse("10:01 3 client3", 3, CustomTime(10,0)));
    manager->processEvent(Event::parse("11:00 4 client1", 3, CustomTime(10,1)));
    
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("10:01 13 ICanWaitNoLonger!"), std::string::npos);
}

TEST_F(ClubManagerTest, AlphabeticalOrderForClosing) {
    manager->processEvent(Event::parse("09:01 1 client3", 3, CustomTime(0,0)));
    manager->processEvent(Event::parse("09:02 1 client1", 3, CustomTime(9,1)));
    manager->processEvent(Event::parse("09:03 1 client2", 3, CustomTime(9,2)));
    
    testing::internal::CaptureStdout();
    manager->closeClub();
    std::string output = testing::internal::GetCapturedStdout();
    
    size_t pos1 = output.find("19:00 11 client1");
    size_t pos2 = output.find("19:00 11 client2");
    size_t pos3 = output.find("19:00 11 client3");
    
    EXPECT_TRUE(pos1 != std::string::npos && 
               pos2 != std::string::npos && 
               pos3 != std::string::npos);
    EXPECT_LT(pos1, pos2);
    EXPECT_LT(pos2, pos3);
}

TEST_F(ClubManagerTest, ClientArrivalAtClosingTime) {
    testing::internal::CaptureStdout();
    manager->processEvent(Event::parse("19:00 1 client1", 3, CustomTime(0,0)));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("19:00 13 NotOpenYet"), std::string::npos);
}

TEST_F(ClubManagerTest, MinuteBeforeClosing) {
    manager->processEvent(Event::parse("18:59 1 client1", 3, CustomTime(0,0)));
    testing::internal::CaptureStdout();
    manager->processEvent(Event::parse("19:00 4 client1", 3, CustomTime(18,59)));
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("13"), std::string::npos); // Нет ошибок
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}