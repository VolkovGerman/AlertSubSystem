#include "json.hpp"
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>

using json = nlohmann::json;

enum Priority {
	HIGH,
	MEDIUM,
	LOW
};

enum Severity {
	CRITICAL,
	ERROR,
	WARNING,
	DEBUG
};

struct Defs {
	long periodicity_time_;
	std::string message_;
	std::vector<std::string> recipients_email_;
	Priority priority_;
	Severity severity_;

	//temporary function, for testing
	int DefsInit(long time, std::string mes, std::vector<std::string> vec, Priority pr, Severity sev) {
		this->periodicity_time_ = time;
		this->message_ = mes;
		this->recipients_email_ = vec;
		this->priority_ = pr;
		this->severity_ = sev;
		return 0;
	}	

	json CreateJsonDefs() {
		json j, jmail;
		std::vector<std::string>::iterator it;
		for (it = recipients_email_.begin(); it != recipients_email_.end(); ++it) {
			jmail.push_back(*it);
		}
		if (this->periodicity_time_ < 0 || this->periodicity_time_ > 2000000) {
			this->periodicity_time_ = -1;
			j["periodicity_time"] = this->periodicity_time_;
		} else {
			j["periodicity_time"] = this->periodicity_time_;
		}
		if (this->message_.empty()) {
			j["message"] = nullptr;
		} else {
			j["message"] = this->message_;
		}
		if (this->recipients_email_.empty()) {
			j["recipients_email"] = nullptr;
		} else {
			j["recipients_email"] = jmail;
		}
		if (this->priority_ < 0 || this->priority_ > 2) {			
			j["priority"] = -1;
		}else {
			j["priority"] = this->priority_;
		}
		if (this->priority_ < 0 || this->priority_ > 3) {
			j["severity"] = -1;
		} else {
			j["severity"] = this->severity_;
		}		
		return j;
	}

	std::string GetStrPrior(int num) {
		if (num == 0) {
			return "HIGH";
		}
		if (num == 1) {
			return "MEDIUM";
		}
		if (num == 2) {
			return "LOW";
		}
	}

	Priority GetPriority(int num) {
		if (num == 0) {
			return HIGH;
		}
		if (num == 1) {
			return MEDIUM;
		}
		if (num == 2) {
			return LOW;
		}		
	}
	std::string GetStrSever(int num) {
		if (num == 0) {
			return "CRITICAL";
		}
		if (num == 1) {
			return "ERROR";
		}
		if (num == 2) {
			return "WARNING";
		}
		if (num == 3) {
			return  "DEBUG";
		}
	}
	Severity GetSeverity(int num) {
		if (num == 0) {
			return CRITICAL;
		}
		if (num == 1) {
			return ERROR;
		}
		if (num == 2) {
			return WARNING;
		}
		if (num == 3) {
			return  DEBUG;
		}		
	}

	int ParseJsonDefs(json d) {
		json j = json::parse(d.dump());				
		for (json::iterator it = j.begin(); it != j.end(); ++it) {
			if (it.key() == "periodicity_time") {
				this->periodicity_time_ = it.value();
			} else if (it.key() == "message") {
				json js = it.value();
				if (!js.is_null()) {
					std::string buf = js;
					this->message_ = buf;
				}
			} else if (it.key() == "recipients_email") {
				json js = it.value();
				if (!js.is_null()) {
					std::vector<std::string> buf = js;
					this->recipients_email_ = buf;
				}
			} else if (it.key() == "priority") {
				this->priority_ = this->GetPriority(it.value());
			} else if (it.key() == "severity") {
				this->severity_ = this->GetSeverity(it.value());
			}
		}
		return 0;
	}
};
//************************************************************ Struct SubKey *******************************************
struct SubKey {
	int subkey_;
	Defs subkey_def_;

	int SubKeyInit(int subkey) {
		this->subkey_ = subkey;
		return 0;
	}

	json CreateJsonSubKey() {
		json j, jsubkey;
		jsubkey = subkey_def_.CreateJsonDefs();
		if (this->subkey_ < 0) {
			j["subkey"] = -1;
		} else {
			j["subkey"] = this->subkey_;
		}
		j["subkey_def"] = jsubkey;

		return j;
	}

	int ParseJsonSubkey(json d) {
		json j = json::parse(d.dump());	
		for (json::iterator it = j.begin(); it != j.end(); ++it) {
			if (it.key() == "subkey") {
				this->subkey_ = it.value();
			} else if (it.key() == "subkey_def") {
				json js = it.value();
				Defs buf;
				buf.ParseJsonDefs(js);
				this->subkey_def_ = buf;
			}
		}
		return 0;
	}
};
//************************************************************ Struct Type *******************************************
struct Type {
	std::string type_;
	Defs type_def_;
	std::vector<SubKey> subkeys_;	

	int TypeInit(std::string type, int subkey) {
		this->type_ = type;
		SubKey buf;
		buf.SubKeyInit(subkey);
		this->subkeys_.push_back(buf);
		return 0;
	}

	int CompareSubKey(int subkey) {
		std::vector<SubKey>::iterator it;
		int z = 0;
		for (it = this->subkeys_.begin(); it != this->subkeys_.end(); ++it) {
			if ((*it).subkey_ == subkey) {
				return z;
			}
			z++;
		}
		return -1;
	}

	json CreateJsonType() {
		std::vector<SubKey>::iterator it;
		json j, jtype, jsvec;		
		jtype = type_def_.CreateJsonDefs();
		if (this->type_.empty()) {
			j["type"] = nullptr;
		} else {
			j["type"] = this->type_;
		}
		j["type_def"] = jtype;
		if (this->subkeys_.empty()) {
			j["subkeys"] = nullptr;
		} else {
			for (it = subkeys_.begin(); it != subkeys_.end(); ++it) {
				json buf = (*it).CreateJsonSubKey();
				jsvec.push_back(buf);
			}
			j["subkeys"] = jsvec;
		}
		return j;
	}

	int ParseJsonType(json d) {
		json j = json::parse(d.dump());
		Defs buf;		
		for (json::iterator it = j.begin(); it != j.end(); ++it) {
			if (it.key() == "type") {
				json js = it.value();
				if (!js.is_null()) {
					std::string buf = js;
					this->type_ = buf;
				}
			} else if (it.key() == "type_def") {
				json js = it.value();
				buf.ParseJsonDefs(js);
				this->type_def_ = buf;
			} else if (it.key() == "subkeys") {
				json jm = it.value();
				if (!jm.is_null()) {
					json::array_t jn = jm;
					for (json::array_t::iterator im = jn.begin(); im != jn.end(); ++im) {
						SubKey sub;
						sub.ParseJsonSubkey(*im);
						this->subkeys_.push_back(sub);
					}
				}
			}
		}
		return 0;
	}
};
//************************************************************ Struct Origin *******************************************
struct Origin {
	std::string origin_;
	Defs origin_def_;
	std::vector<Type> types_;

	int OriginInit(std::string origin, std::string type, int subkey) {
		this->origin_ = origin;
		Type buf;
		buf.TypeInit(type, subkey);
		this->types_.push_back(buf);
		return 0;
	}

	int CompareType(std::string type) {
		std::vector<Type>::iterator it;
		int z = 0;
		for (it = this->types_.begin(); it != this->types_.end(); ++it) {
			if ((*it).type_ == type) {
				return z;
			}
			z++;
		}
		return -1;
	}

	json CreateJsonOrigin() {
		json j, jorigin, jsvec;		
		std::vector<Type>::iterator it;
		jorigin = origin_def_.CreateJsonDefs();

		if (this->origin_.empty()) {
			j["origin"] = nullptr;
		} else {
			j["origin"] = this->origin_;
		}
		j["origin_def"] = jorigin;
		if (this->types_.empty()) {
			j["types"] = nullptr;
		} else {
			for (it = types_.begin(); it != types_.end(); ++it) {
				json buf = (*it).CreateJsonType();
				jsvec.push_back(buf);
			}
			j["types"] = jsvec;
		}
		return j;
	}

	int ParseJsonOrigin(json d) {
		json j = json::parse(d.dump());
		Defs buf;
		
		for (json::iterator it = j.begin(); it != j.end(); ++it) {
			if (it.key() == "origin") {
				json js = it.value();
				if (!js.is_null()) {
					std::string buf = js;
					this->origin_ = buf;
				}
			} else if (it.key() == "origin_def") {
				json js = it.value();
				buf.ParseJsonDefs(js);
				this->origin_def_ = buf;
			} else if (it.key() == "types") {
				json jm = it.value();
				if (!jm.is_null()) {
					json::array_t jn = jm;
					for (json::array_t::iterator im = jn.begin(); im != jn.end(); ++im) {
						Type sub;
						sub.ParseJsonType(*im);
						this->types_.push_back(sub);
					}
				}
			}
		}
		return 0;
	}


};
//************************************************************ Config Data *******************************************
struct ConfigData {
	int max_alerts_;
	Defs global_def_;
	std::vector<Origin> origins_;

	std::string GetMessage() {
		return this->global_def_.message_;
	}

	std::string GetMessage(std::string origin) {
		int origin_index = this->CompareOrigin(origin);	
		if (origin_index >= 0) {
			if (!this->origins_[origin_index].origin_def_.message_.empty()) {
				return this->origins_[origin_index].origin_def_.message_;
			} else {
				return this->GetMessage();
			}
		}
		return this->GetMessage();
	}
	
	std::string GetMessage(std::string origin, std::string type) {
		int origin_index = this->CompareOrigin(origin);		
		if (origin_index >= 0) {
			int type_index = this->origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				if (!this->origins_[origin_index].types_[type_index].type_def_.message_.empty()) {
					return this->origins_[origin_index].types_[type_index].type_def_.message_;
				} else {
					if (!this->origins_[origin_index].origin_def_.message_.empty()) {					  
						return this->origins_[origin_index].origin_def_.message_;
					} else {
						return this->GetMessage();
					}
				}				
			}
			return this->GetMessage();
		}
	}

	std::string GetMessage(std::string origin, std::string type, int subkey) {
		int origin_index = this->CompareOrigin(origin);	
		if (origin_index >= 0) {
			int type_index = this->origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				int subkey_index = this->origins_[origin_index].types_[type_index].CompareSubKey(subkey);
				if (subkey_index >= 0) {
					if (!this->origins_[origin_index].types_[type_index].
						subkeys_[subkey_index].subkey_def_.message_.empty()) {
						return this->origins_[origin_index].types_[type_index].
							subkeys_[subkey_index].subkey_def_.message_;
					} else {
						if (!this->origins_[origin_index].types_[type_index].type_def_.message_.empty()) {
							return this->origins_[origin_index].types_[type_index].type_def_.message_;
						} else {
							if (!this->origins_[origin_index].origin_def_.message_.empty()) {
								return this->origins_[origin_index].origin_def_.message_;
							} else {
								return this->GetMessage();
							}
						}
					}								
				}
			}
		}
		return this->GetMessage();
	}

	long GetPeriodicity() {
		return this->global_def_.periodicity_time_;
	}

	long GetPeriodicity(std::string origin) {
		int origin_index = this->CompareOrigin(origin);		
		if (origin_index >= 0) {
			if (this->origins_[origin_index].origin_def_.periodicity_time_ >= 0) {
				return this->origins_[origin_index].origin_def_.periodicity_time_;
			} else {
				return this->GetPeriodicity();
			}
		}
		return this->GetPeriodicity();
	}

	long GetPeriodicity(std::string origin, std::string type) {
		int origin_index = this->CompareOrigin(origin);		
		if (origin_index >= 0) {
			int type_index = this->origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				if (this->origins_[origin_index].types_[type_index].type_def_.periodicity_time_ >= 0) {
					return this->origins_[origin_index].types_[type_index].type_def_.periodicity_time_;
				} else {
					if (this->origins_[origin_index].origin_def_.periodicity_time_ >= 0) {
						return this->origins_[origin_index].origin_def_.periodicity_time_;
					} else {
						return this->GetPeriodicity();
					}
				}
			}
			return this->GetPeriodicity();
		}
	}

	long GetPeriodicity(std::string origin, std::string type, int subkey) {
		int origin_index = this->CompareOrigin(origin);		
		if (origin_index >= 0) {
			int type_index = this->origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				int subkey_index = this->origins_[origin_index].types_[type_index].CompareSubKey(subkey);
				if (subkey_index >= 0) {
					if (this->origins_[origin_index].types_[type_index].
						subkeys_[subkey_index].subkey_def_.periodicity_time_ >= 0) {
						return this->origins_[origin_index].types_[type_index].
							subkeys_[subkey_index].subkey_def_.periodicity_time_;
					} else {
						if (this->origins_[origin_index].types_[type_index].type_def_.periodicity_time_ >= 0) {
							return this->origins_[origin_index].types_[type_index].type_def_.periodicity_time_;
						} else {
							if (this->origins_[origin_index].origin_def_.periodicity_time_ >= 0) {
								return this->origins_[origin_index].origin_def_.periodicity_time_;
							} else {
								return this->GetPeriodicity();
							}
						}
					}
				}
			}
		}
		return this->GetPeriodicity();
	}

	Priority GetPriority() {
		Defs buf;		
		return buf.GetPriority(this->global_def_.priority_);
	}
	
	Priority GetPriority(std::string origin) {
		int origin_index = this->CompareOrigin(origin);
		Defs buf;
		if (origin_index >= 0) {
			if (this->origins_[origin_index].origin_def_.priority_ >= 0) {
				return buf.GetPriority(this->origins_[origin_index].origin_def_.priority_);
			} else {
				return this->GetPriority();
			}
		}
		return this->GetPriority();
	}
	
	Priority GetPriority(std::string origin, std::string type) {
		int origin_index = this->CompareOrigin(origin);
		Defs buf;
		if (origin_index >= 0) {
			int type_index = this->origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				if (this->origins_[origin_index].types_[type_index].type_def_.priority_ >= 0) {
					return buf.GetPriority(this->origins_[origin_index].types_[type_index].type_def_.priority_);
				} else {
					if (this->origins_[origin_index].origin_def_.priority_ >= 0) {
						return buf.GetPriority(this->origins_[origin_index].origin_def_.priority_);
					} else {
						return this->GetPriority();
					}
				}
			}
			return this->GetPriority();
		}
	}

	Priority GetPriority(std::string origin, std::string type, int subkey) {
		int origin_index = this->CompareOrigin(origin);
		Defs buf;
		if (origin_index >= 0) {
			int type_index = this->origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				int subkey_index = this->origins_[origin_index].types_[type_index].CompareSubKey(subkey);
				if (subkey_index >= 0) {
					if (this->origins_[origin_index].types_[type_index].
						subkeys_[subkey_index].subkey_def_.priority_ >= 0) {
						return buf.GetPriority(this->origins_[origin_index].types_[type_index].
							subkeys_[subkey_index].subkey_def_.priority_);
					} else {
						if (this->origins_[origin_index].types_[type_index].type_def_.priority_ >= 0) {
							return buf.GetPriority(this->origins_[origin_index].types_[type_index].type_def_.priority_);
						} else {
							if (this->origins_[origin_index].origin_def_.priority_ >= 0) {
								return buf.GetPriority(this->origins_[origin_index].origin_def_.priority_);
							} else {
								return this->GetPriority();
							}
						}
					}
				}
			}
		}
		return this->GetPriority();
	}

	Severity GetSeverity() {
		Defs buf;
		return buf.GetSeverity(this->global_def_.severity_);
	}

	Severity GetSeverity(std::string origin) {
		int origin_index = this->CompareOrigin(origin);
		Defs buf;
		if (origin_index >= 0) {
			if (this->origins_[origin_index].origin_def_.severity_ >= 0) {
				return buf.GetSeverity(this->origins_[origin_index].origin_def_.severity_);
			} else {
				return this->GetSeverity();
			}
		}
		return this->GetSeverity();
	}

	Severity GetSeverity(std::string origin, std::string type) {
		int origin_index = this->CompareOrigin(origin);
		Defs buf;
		if (origin_index >= 0) {
			int type_index = this->origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				if (this->origins_[origin_index].types_[type_index].type_def_.severity_ >= 0) {
					return buf.GetSeverity(this->origins_[origin_index].types_[type_index].type_def_.severity_);
				} else {
					if (this->origins_[origin_index].origin_def_.severity_ >= 0) {
						return buf.GetSeverity(this->origins_[origin_index].origin_def_.severity_);
					} else {
						return this->GetSeverity();
					}
				}
			}
			return this->GetSeverity();
		}
	}

	Severity GetSeverity(std::string origin, std::string type, int subkey) {
		int origin_index = this->CompareOrigin(origin);
		Defs buf;
		if (origin_index >= 0) {
			int type_index = this->origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				int subkey_index = this->origins_[origin_index].types_[type_index].CompareSubKey(subkey);
				if (subkey_index >= 0) {
					if (this->origins_[origin_index].types_[type_index].
						subkeys_[subkey_index].subkey_def_.severity_ >= 0) {
						return buf.GetSeverity(this->origins_[origin_index].types_[type_index].
							subkeys_[subkey_index].subkey_def_.severity_);
					} else {
						if (this->origins_[origin_index].types_[type_index].type_def_.severity_ >= 0) {
							return buf.GetSeverity(this->origins_[origin_index].types_[type_index].type_def_.severity_);
						} else {
							if (this->origins_[origin_index].origin_def_.severity_ >= 0) {
								return buf.GetSeverity(this->origins_[origin_index].origin_def_.severity_);
							} else {
								return this->GetSeverity();
							}
						}
					}
				}
			}
		}
		return this->GetSeverity();
	}
	
	std::vector<std::string> GetRecipientsEmail() {
		return this->global_def_.recipients_email_;
	}

	std::vector<std::string> GetRecipientsEmail(std::string origin) {
		int origin_index = this->CompareOrigin(origin);
		if (origin_index >= 0) {
			if (!this->origins_[origin_index].origin_def_.recipients_email_.empty()) {
				return this->origins_[origin_index].origin_def_.recipients_email_;
			} else {
				return this->GetRecipientsEmail();
			}
		}
		return this->GetRecipientsEmail();
	}

	std::vector<std::string> GetRecipientsEmail(std::string origin, std::string type) {
		int origin_index = this->CompareOrigin(origin);
		if (origin_index >= 0) {
			int type_index = this->origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				if (!this->origins_[origin_index].types_[type_index].type_def_.recipients_email_.empty()) {
					return this->origins_[origin_index].types_[type_index].type_def_.recipients_email_;
				} else {
					if (!this->origins_[origin_index].origin_def_.recipients_email_.empty()) {
						return this->origins_[origin_index].origin_def_.recipients_email_;
					} else {
						return this->GetRecipientsEmail();
					}
				}
			}
			return this->GetRecipientsEmail();
		}
	}

	std::vector<std::string> GetRecipientsEmail(std::string origin, std::string type, int subkey) {
		int origin_index = this->CompareOrigin(origin);
		if (origin_index >= 0) {
			int type_index = this->origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				int subkey_index = this->origins_[origin_index].types_[type_index].CompareSubKey(subkey);
				if (subkey_index >= 0) {
					if (!this->origins_[origin_index].types_[type_index].
						subkeys_[subkey_index].subkey_def_.recipients_email_.empty()) {
						return this->origins_[origin_index].types_[type_index].
							subkeys_[subkey_index].subkey_def_.recipients_email_;
					} else {
						if (!this->origins_[origin_index].types_[type_index].type_def_.recipients_email_.empty()) {
							return this->origins_[origin_index].types_[type_index].type_def_.recipients_email_;
						} else {
							if (!this->origins_[origin_index].origin_def_.recipients_email_.empty()) {
								return this->origins_[origin_index].origin_def_.recipients_email_;
							} else {
								return this->GetRecipientsEmail();
							}
						}
					}
				}
			}
		}
		return this->GetRecipientsEmail();
	}

	int CompareOrigin(std::string origin) {
		std::vector<Origin>::iterator it;
		int z = 0;
		for (it = this->origins_.begin(); it != this->origins_.end(); ++it) {
			if ((*it).origin_ == origin) {
				return z;
			}
			z++;
		}
		return -1;
	}

	json CreateJsonConfigD() {
		json j, jconfdata, jsvec;
		Defs global_def;
		std::vector<Origin>::iterator it;
		jconfdata = global_def_.CreateJsonDefs();
		if (this->max_alerts_ < 0) {
			j["max_alert"] = -1;
		} else {
			j["max_alert"] = this->max_alerts_;
		}
		j["global_def"] = jconfdata;
		if (this->origins_.empty()) {
			j["origins"] = nullptr;
		} else {
			for (it = origins_.begin(); it != origins_.end(); ++it) {
				json buf = (*it).CreateJsonOrigin();
				jsvec.push_back(buf);
			}
			j["origins"] = jsvec;
		}
		return j;
	}

	int ParseJsonConfigD(json d) {
		json j = json::parse(d.dump());
		Defs buf;		
		for (json::iterator it = j.begin(); it != j.end(); ++it) {
			if (it.key() == "max_alert") {
				this->max_alerts_ = it.value();
			} else if (it.key() == "global_def") {
				json js = it.value();
				buf.ParseJsonDefs(js);
				this->global_def_ = buf;
			} else if (it.key() == "origins") {
				json jm = it.value();
				if (!jm.is_null()) {
					json::array_t jn = jm;
					for (json::array_t::iterator im = jn.begin(); im != jn.end(); ++im) {
						Origin sub;
						sub.ParseJsonOrigin(*im);
						this->origins_.push_back(sub);
					}
				}
			}
		}
		return 0;
	}
};
//************************************************************ Class Config *******************************************
class Config {
private:
	ConfigData data_;
	std::string config_file_ = "config.json";
public:
	int Empty() {
		if (this->data_.origins_.empty()) {
			return 1;
		}
		return 0;
	}

	int ReadConfig() {
		std::fstream outfile;
		outfile.open(this->config_file_.c_str(), std::ios::in);
		if (!outfile.is_open()) {
			outfile.close();
			return 0;
		}
		std::string buf;
		outfile >> buf;
		outfile.close();
		json jdata = json::parse(buf);
		this->data_.ParseJsonConfigD(jdata);
		return 1;
	}

	int WriteConfig() {
		std::fstream outfile;
		std::string buf;
		outfile.open(this->config_file_.c_str(), std::ios::out);
		if (!outfile.is_open()) {
			outfile.close();
			return 0;
		}
		buf = this->data_.CreateJsonConfigD().dump();
		// write inputted data into the file.
		outfile << buf;
		outfile.close();
		return 1;
	}
	//temporary function for testing
	int CompareType(int orig_index, std::string type) {
		std::vector<Type>::iterator it;
		int z = 0;
		for (it = this->data_.origins_[orig_index].types_.begin(); it != this->data_.origins_[orig_index].types_.end(); ++it) {
			if ((*it).type_ == type) {
				return z;
			}
			z++;
		}
		return -1;
	}
	//temporary function for testing
	int CompareSubKey(int orig_index, int type_index, int subkey) {
		std::vector<SubKey>::iterator it;
		int z = 0;
		for (it = this->data_.origins_[orig_index].types_[type_index].subkeys_.begin(); it != this->data_.origins_[orig_index].types_[type_index].subkeys_.end(); ++it) {
			if ((*it).subkey_ == subkey) {
				return z;
			}
			z++;
		}
		return -1;
	}
	//temporary function for testing
	int MakeNote() {
		std::string origin, type;
		int subkey;

		std::cout << "Enter origin: ";
		std::cin >> origin;
		std::cout << std::endl;
		std::cout << "Enter type: ";
		std::cin >> type;
		std::cout << std::endl;
		std::cout << "Enter subkey: ";
		std::cin >> subkey;
		std::cout << std::endl;

		if (this->CheckNote(origin, type, subkey)) {
			this->WriteConfig();
		}

		return 0;
	}

	int CheckNote(std::string origin, std::string type, int subkey) {
		int origin_index, type_index;

		origin_index = this->data_.CompareOrigin(origin);
		if (origin_index >= 0) {
			type_index = this->CompareType(origin_index, type);
			if (type_index >= 0) {
				if (this->CompareSubKey(origin_index, type_index, subkey) >= 0) {
					return 0;
				} else {
					SubKey buf_subkey;
					buf_subkey.SubKeyInit(subkey);
					this->data_.origins_[origin_index].types_[type_index].subkeys_.push_back(buf_subkey);
					return 1;
				}
			} else {
				Type buf_type;
				buf_type.TypeInit(type, subkey);
				this->data_.origins_[origin_index].types_.push_back(buf_type);
				return 1;
			}
		} else {
			Origin buf_origin;
			buf_origin.OriginInit(origin, type, subkey);
			this->data_.origins_.push_back(buf_origin);
		}
		return 1;
	}
	
	int MakeTable() {
		std::string origin = "originnnnnnnnnnnnnnnnnnnnnnnnnnnnn", type = "type",subkey = "subkey",num = "num.";
		 
		for (int i = 0; i < 80; i++) {
			std::cout << '-';
		}
		//std::cout << "|" << std::setw(10) << num << "|" << std::setw(27) << origin << "|" << std::setw(27) << type << "|" << std::setw(11) << subkey << "|";
		std::cout.width(10);
		std::cout << origin;
		for (int i = 0; i < 80; i++) {
			std::cout << '-';
		}
		return 0;
	}

	int SetGlobal() {
		this->data_.global_def_.message_ = "global_def";
		this->data_.global_def_.periodicity_time_ = 228;
		this->data_.global_def_.priority_ = HIGH;
		this->data_.global_def_.severity_ = CRITICAL;
		this->data_.global_def_.recipients_email_.push_back("global_email");
		return 0;
	}

	//temporary function for testing
	int EnterInf() {
		std::string buf;
		std::cout << "Enter global message: ";
		std::cin >> buf;
		this->data_.global_def_.message_ = buf;
		return 0;
	}

	int EnterInf(std::string origin) {
		int origin_index = this->data_.CompareOrigin(origin);
		if (origin_index >= 0) {
			std::string buf;
			std::cout << "Enter origin message: ";
			std::cin >> buf;
			this->data_.origins_[origin_index].origin_def_.message_ = buf;
			return 1;
		}
		std::cout << "this origin is not found" << std::endl;
		return 0;
	}

	int EnterInf(std::string origin, std::string type) {
		int origin_index = this->data_.CompareOrigin(origin);
		if (origin_index >= 0) {
			int type_index = this->data_.origins_[origin_index].CompareType(type);
				if (type_index >= 0) {
					std::string buf;
					std::cout << "Enter type message: ";
					std::cin >> buf;
					this->data_.origins_[origin_index].types_[type_index].type_def_.message_ = buf;
					return 1;
				}				
		}
		std::cout << "this origin is not found" << std::endl;
		return 0;	
	}

	int EnterInf(std::string origin,std::string type,int subkey) {
		int origin_index = this->data_.CompareOrigin(origin);
		if (origin_index >= 0) {
			int type_index = this->data_.origins_[origin_index].CompareType(type);
			if (type_index >= 0) {
				int subkey_index = this->data_.origins_[origin_index].types_[type_index].CompareSubKey(subkey);
				if (subkey_index >= 0) {
					std::string buf;
					std::cout << "Enter user message: ";
					std::cin >> buf;
					this->data_.origins_[origin_index].types_[type_index].subkeys_[subkey_index].subkey_def_.message_ = buf;
					return 1;
				}				
			}
		}
		std::cout << "this origin is not found" << std::endl;
		return 0;
	}
	
	std::string GetMessage() {
		std::string buf = this->data_.GetMessage();		
		return buf;
	}

	std::string GetMessage(std::string origin) {
		std::string buf = this->data_.GetMessage(origin);		
		return buf;
	}
	std::string GetMessage(std::string origin, std::string type) {
		std::string buf = this->data_.GetMessage(origin, type);	
		return buf;
	}
	std::string GetMessage(std::string origin, std::string type, int subkey) {
		std::string buf = this->data_.GetMessage(origin, type, subkey);	 
		return buf;
	}

	long GetPeriodicity() {
		long time = this->data_.GetPeriodicity();
		return time;
	}

	long GetPeriodicity(std::string origin) {
		long time = this->data_.GetPeriodicity(origin);
		return time;
	}

	long GetPeriodicity(std::string origin, std::string type) {
		long time = this->data_.GetPeriodicity(origin, type);
		return time;
	}

	long GetPeriodicity(std::string origin, std::string type, int subkey) {
		long time = this->data_.GetPeriodicity(origin, type, subkey);
		return time;
	}

	Priority GetPriority() {
		return this->data_.GetPriority();
	}

	Priority GetPriority(std::string origin) {
		return this->data_.GetPriority(origin);
	}

	Priority GetPriority(std::string origin, std::string type) {
		return this->data_.GetPriority(origin, type);
	}

	Priority GetPriority(std::string origin, std::string type, int subkey) {
		return this->data_.GetPriority(origin, type, subkey);
	}

	Severity GetSeverity() {
		return this->data_.GetSeverity();
	}

	Severity GetSeverity(std::string origin) {
		return this->data_.GetSeverity(origin);
	}

	Severity GetSeverity(std::string origin, std::string type) {
		return this->data_.GetSeverity(origin, type);
	}

	Severity GetSeverity(std::string origin, std::string type, int subkey) {
		return this->data_.GetSeverity(origin, type, subkey);
	}

	std::vector<std::string> GetRecipientsEmail() {
		return this->data_.GetRecipientsEmail();
	}

	std::vector<std::string> GetRecipientsEmail(std::string origin) {
		return this->data_.GetRecipientsEmail(origin);
	}

	std::vector<std::string> GetRecipientsEmail(std::string origin, std::string type) {
		return this->data_.GetRecipientsEmail(origin, type);
	}

	std::vector<std::string> GetRecipientsEmail(std::string origin, std::string type, int subkey) {
		return this->data_.GetRecipientsEmail(origin, type, subkey);
	}
};