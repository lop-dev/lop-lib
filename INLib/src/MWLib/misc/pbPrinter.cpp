//////////////////////////////////////////////////////////////////////
//  created:    2016/06/05
//  filename:   SHLib/utility/pb2jsonPrinter.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <MWLib/misc/pbPrinter.h>

#undef GetMessage

namespace MWLib
{
    namespace Misc
    {
        BCLIB_SINGLETON_DEFINE(CPBPrinter)

        CPBPrinter::CPBPrinter()
        {
        }

        CPBPrinter::~CPBPrinter()
        {
        }

        void CPBPrinter::_printMessages(std::ostream& os, const google::protobuf::Message &Message_P, const google::protobuf::Descriptor *pDescriptor, const google::protobuf::Reflection *pReflection, int deepnum, bool format)
        {
            google::protobuf::Descriptor *pSubDescriptor = NULL;
            google::protobuf::FieldDescriptor *pFieldDescriptor = NULL;
            google::protobuf::EnumValueDescriptor *pEnumValueDes = NULL;
            bool bRepeated;
            int  FieldNum;
            int  FieldSize;
            std::string spaces((deepnum - 1) * 2, ' ');
            std::string one_spaces = "  ";
            std::string char_return = "\n";
            if (!format)
            {
                spaces = "";
                one_spaces = "";
                char_return = "";
            }

            bool isfirstfield = true;
            os << char_return << spaces << "{";
            for (int i = 0; i < pDescriptor->field_count(); i++) {

                pFieldDescriptor = (google::protobuf::FieldDescriptor *)pDescriptor->field(i);
                bRepeated = pFieldDescriptor->is_repeated();

                //没有数据就不输出该字段
                if (bRepeated)
                {
                    //有重复的项,如果size为0 则不输出
                    FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);
                    if (FieldSize <= 0)
                    {
                        continue;
                    }
                }
                else
                {
                    //单项,如果没数据 则不输出
                    if (!pReflection->HasField(Message_P, pFieldDescriptor))
                    {
                        continue;
                    }
                }

                if (isfirstfield)
                {
                    isfirstfield = false;
                    os << char_return;	//第一个字段前面不用加,
                }
                else
                {
                    os << "," << char_return;	//其他的要加,分隔
                }

                os << spaces << one_spaces << "\"" << pFieldDescriptor->name() << "\": ";

                switch (pFieldDescriptor->type())
                {
                case google::protobuf::FieldDescriptor::TYPE_FIXED32:
                case google::protobuf::FieldDescriptor::TYPE_INT32:
                case google::protobuf::FieldDescriptor::TYPE_SINT32:
                case google::protobuf::FieldDescriptor::TYPE_SFIXED32:
                    if (!bRepeated) {
                        os << pReflection->GetInt32(Message_P, pFieldDescriptor);
                    }
                    else {
                        os << "[";
                        FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);
                        for (FieldNum = 0; FieldNum < FieldSize; FieldNum++) {
                            os << pReflection->GetRepeatedInt32(Message_P, pFieldDescriptor, FieldNum);
                            if (FieldNum != FieldSize - 1)
                                os << ",";
                        }
                        os << "]";
                    }
                    break;
                case google::protobuf::FieldDescriptor::TYPE_UINT32:
                    if (!bRepeated) {
                        os << pReflection->GetUInt32(Message_P, pFieldDescriptor);
                    }
                    else {
                        os << "[";
                        FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);
                        for (FieldNum = 0; FieldNum < FieldSize; FieldNum++) {
                            os << pReflection->GetRepeatedUInt32(Message_P, pFieldDescriptor, FieldNum);
                            if (FieldNum != FieldSize - 1)
                                os << ",";
                        }
                        os << "]";
                    }
                    break;
                case google::protobuf::FieldDescriptor::TYPE_FIXED64:
                case google::protobuf::FieldDescriptor::TYPE_INT64:
                case google::protobuf::FieldDescriptor::TYPE_SINT64:
                case google::protobuf::FieldDescriptor::TYPE_SFIXED64:
                {
                    if (!bRepeated) {
                        os << pReflection->GetInt64(Message_P, pFieldDescriptor);
                    }
                    else {
                        os << "[";
                        FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);
                        for (FieldNum = 0; FieldNum < FieldSize; FieldNum++) {
                            os << pReflection->GetRepeatedInt64(Message_P, pFieldDescriptor, FieldNum);
                            if (FieldNum != FieldSize - 1)
                                os << ",";
                        }
                        os << "]";
                    }
                    break;
                }
                case google::protobuf::FieldDescriptor::TYPE_UINT64:
                {
                    if (!bRepeated) {
                        os << (unsigned long long)pReflection->GetUInt64(Message_P, pFieldDescriptor);
                    }
                    else {
                        os << "[";
                        FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);
                        for (FieldNum = 0; FieldNum < FieldSize; FieldNum++) {
                            os << (unsigned long long)pReflection->GetRepeatedUInt64(Message_P, pFieldDescriptor, FieldNum);
                            if (FieldNum != FieldSize - 1)
                                os << ",";
                        }
                        os << "]";
                    }
                    break;
                }
                case google::protobuf::FieldDescriptor::TYPE_FLOAT:
                {
                    if (!bRepeated) {
                        os << pReflection->GetFloat(Message_P, pFieldDescriptor);
                    }
                    else {
                        os << "[";
                        FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);
                        for (FieldNum = 0; FieldNum < FieldSize; FieldNum++) {
                            os << pReflection->GetRepeatedFloat(Message_P, pFieldDescriptor, FieldNum);
                            if (FieldNum != FieldSize - 1)
                                os << ",";
                        }
                        os << "]";
                    }
                    break;
                }
                case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
                {
                    if (!bRepeated) {
                        os << pReflection->GetDouble(Message_P, pFieldDescriptor);
                    }
                    else {
                        os << "[";
                        FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);
                        for (FieldNum = 0; FieldNum < FieldSize; FieldNum++) {
                            os << pReflection->GetRepeatedDouble(Message_P, pFieldDescriptor, FieldNum);
                            if (FieldNum != FieldSize - 1)
                                os << ",";
                        }
                        os << "]";
                    }
                    break;
                }
                case google::protobuf::FieldDescriptor::TYPE_BOOL:
                    if (!bRepeated) {
                        os << pReflection->GetBool(Message_P, pFieldDescriptor);
                    }
                    else {
                        os << "[";
                        FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);
                        for (FieldNum = 0; FieldNum < FieldSize; FieldNum++) {
                            os << pReflection->GetRepeatedBool(Message_P, pFieldDescriptor, FieldNum);
                            if (FieldNum != FieldSize - 1)
                                os << ",";
                        }
                        os << "]";
                    }
                    break;
                case google::protobuf::FieldDescriptor::TYPE_ENUM:
                    if (!bRepeated) {
                        pEnumValueDes = (google::protobuf::EnumValueDescriptor *)pReflection->GetEnum(Message_P, pFieldDescriptor);
                        os << pEnumValueDes->number();
                    }
                    else {
                        os << "[";
                        FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);
                        for (FieldNum = 0; FieldNum < FieldSize; FieldNum++) {
                            pEnumValueDes = (google::protobuf::EnumValueDescriptor *)pReflection->GetRepeatedEnum(Message_P, pFieldDescriptor, FieldNum);
                            os << pEnumValueDes->number();
                            if (FieldNum != FieldSize - 1)
                                os << ",";
                        }
                        os << "]";
                    }
                    break;
                case google::protobuf::FieldDescriptor::TYPE_STRING:
                case google::protobuf::FieldDescriptor::TYPE_BYTES:
                    if (!bRepeated) {
                        os << "\"" << pReflection->GetString(Message_P, pFieldDescriptor) << "\"";
                    }
                    else {
                        os << "[";
                        FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);
                        for (FieldNum = 0; FieldNum < FieldSize; FieldNum++) {
                            os << "\"" << pReflection->GetRepeatedString(Message_P, pFieldDescriptor, FieldNum) << "\"";
                            if (FieldNum != FieldSize - 1)
                                os << ",";
                        }
                        os << "]";
                    }
                    break;
                case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
                    pSubDescriptor = (google::protobuf::Descriptor *)(pFieldDescriptor->message_type());

                    if (!bRepeated) {
                        const google::protobuf::Message &child_message = pReflection->GetMessage(Message_P, pFieldDescriptor);
                        const google::protobuf::Reflection *child_reflection = child_message.GetReflection();
                        _printMessages(os, (google::protobuf::Message&)child_message, pSubDescriptor, child_reflection, deepnum + 1, format); //回调
                    }
                    else {
                        os << "[";
                        FieldSize = pReflection->FieldSize(Message_P, pFieldDescriptor);

                        for (FieldNum = 0; FieldNum < FieldSize; FieldNum++) {
                            const google::protobuf::Message &child_message = pReflection->GetRepeatedMessage(Message_P, pFieldDescriptor, FieldNum);
                            const google::protobuf::Reflection *child_reflection = child_message.GetReflection();
                            _printMessages(os, (google::protobuf::Message&)child_message, pSubDescriptor, child_reflection, deepnum + 1, format); //回调
                            if (FieldNum != FieldSize - 1)
                                os << ",";
                        }
                        if (FieldSize > 0)
                            os << char_return << spaces << one_spaces;
                        os << "]";
                    }

                    break;
                default:
                    break;
                }
            }
            os << char_return << spaces << "}";
        }
    }//Utility
}//SHLib
