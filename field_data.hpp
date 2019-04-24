#ifndef _P_SYSTEM_H_
#define _P_SYSTEM_H_
class fielddata {
   public:
    fielddata();
    ~fielddata();
    void ReleaseData();
    void ResetData();

   private:
    std::type_info& type;
};

#endif